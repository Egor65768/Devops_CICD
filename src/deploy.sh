#!/bin/bash

SSH_USER="deploy" 
SSH_HOST="192.168.1.104"  
TARGET_DIR="/usr/local/bin"

echo "Deploy start"

scp -r "$(pwd)/cat" $SSH_USER@$SSH_HOST:$TARGET_DIR
scp -r "$(pwd)/grep" $SSH_USER@$SSH_HOST:$TARGET_DIR

if [ $? -ne 0 ]; then
    echo "Fail deploy"
    exit 1
fi

echo "Sucsses deploy"
