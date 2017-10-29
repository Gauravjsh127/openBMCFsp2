#!/bin/bash

username="gaurav"
frootName="froot8_1-x86_64-mcp8_1-x86_64-mcp-2016-02-05-083333-ppcnf-default32-ppc476workaround-v4-exportroot-PSCN-v170406"

mount --bind /home/$username /zfw_toolkits/$frootName/home/$username
mount --bind /afs /zfw_toolkits/$frootName/afs
mount --bind /proc /zfw_toolkits/$frootName/proc
mount --bind /dev /zfw_toolkits/$frootName/dev
mount --bind /tmp /zfw_toolkits/$frootName/tmp
