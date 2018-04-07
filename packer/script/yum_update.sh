#!/bin/bash
#============================================================================
# Description: This script just perform a yum update. This purpose of this
#		script is for visibility of when a yum update occured durring a
#		provisioning process
#============================================================================

echo "==> performing yum update."
yum update -y