#!/bin/bash

# environment setup:
# RUN_AS - format: <numeric>:<numeric>

# DOCKERBUILD_FETCH_REPO - optional (to fetch repository)
# DOCKERBUILD_FETCH_REFSPEC - optional (to fetch refspec)

# BUILDDIR - path important (default /build)

# persistent data volumes layout (for integration build):
# volume name:   container mountpoint:
# persisten_$uid /persistent/<git repo> - git repo cache
# ccache_$iud    /ccache                - ccache cache
# -              /source                - git local repo (anonymous temp volume)
# <host folder>  /build                 - user owned - bind-mounted from the host

set -e -u

if [ ! -z "${DOCKERBUILD_DEBUG-}" ]; then
    set -x
    export GIT_TRACE=2
    export GIT_TRACE_SETUP=2
    export GIT_TRACE_CURL=2
    export GIT_CURL_VERBOSE=2
    git --version
    git lfs version
fi

SOURCEDIR=${SOURCEDIR:-${PWD}}

RUN_AS=${RUN_AS:-"1000:1000"}
RUN_AS_UID=${RUN_AS%:*}
RUN_AS_GID=${RUN_AS##*:}
unset RUN_AS
echo "running as uid: $RUN_AS_UID gid: $RUN_AS_GID cwd: $PWD"

if [ -z "${RUN_AS_UID##*[!0-9]*}" -o -z "${RUN_AS_GID##*[!0-9]*}" ]; then
    echo 'RUN_AS variable should contain numerical values, i.e. "1000:1000"'
    exit 1
fi

id -gn "$RUN_AS_GID" >/dev/null 2>&1 || groupadd -f -g "$RUN_AS_GID" -o user
id -un "$RUN_AS_UID" >/dev/null 2>&1 || useradd --shell /bin/bash -u "$RUN_AS_UID" -g "$RUN_AS_GID" -o -c "" -m user
export HOME=/home/user
asuser="/usr/sbin/exec-as $RUN_AS_UID $RUN_AS_GID"

if [ ! -z "${DOCKERBUILD_FETCH_REPO-}" ]; then
    echo "trying to fetch $DOCKERBUILD_FETCH_REFSPEC from $DOCKERBUILD_FETCH_REPO"
    GIT_CREDENTIALS="/secret.fifo"
    if [ ! -p "$GIT_CREDENTIALS" ]; then
        >&2 echo "$GIT_CREDENTIALS credentials fifo does not exist"
        exit 1
    fi

    # set up ssh agent and git credentials
    $asuser /bin/bash -c "ssh-agent -s > /tmp/ssh-agent.env"
    source /tmp/ssh-agent.env
    $asuser ssh-add "$GIT_CREDENTIALS"
    $asuser git credential approve < "$GIT_CREDENTIALS"

    # set up data volumes permissions
    chown "$RUN_AS_UID:$RUN_AS_GID" "$SOURCEDIR"
    chown "$RUN_AS_UID:$RUN_AS_GID" "/persistent"
    PERSISTENT_GITREPO="/persistent/${DOCKERBUILD_FETCH_REPO##*/}"
    [[ "$PERSISTENT_GITREPO" == *".git" ]] || PERSISTENT_GITREPO+=".git"

    if [ ! -d "$PERSISTENT_GITREPO" ]; then
        # clone repository
        $asuser git clone --bare "$DOCKERBUILD_FETCH_REPO" "$PERSISTENT_GITREPO"
    else
        # remove worktree metadata older than 3 hours
        find "$PERSISTENT_GITREPO/worktrees" -mindepth 1 -maxdepth 1 -type d -mmin +360 -exec rm -r "{}" \;
    fi

    # set up local git repo worktree
    cd "$PERSISTENT_GITREPO"
    $asuser git worktree add -f --lock --detach --no-checkout "$SOURCEDIR"
    cd "$SOURCEDIR"
    $asuser git fetch origin "$DOCKERBUILD_FETCH_REFSPEC"
    $asuser git checkout FETCH_HEAD || (
        pstree -a
        git lfs logs last
        git lfs env
        exit 1
    )
fi

if [ ! -z "${CCACHE_DIR-}" ]; then
    chown "$RUN_AS_UID:$RUN_AS_GID" "$CCACHE_DIR"
    $asuser ccache -M 2G
fi

exec $asuser "${@}"
