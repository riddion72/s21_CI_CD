#!/bin/bash

REMOTE_HOST="10.20.0.20"
REMOTE_USER="samsondeploy"

scp -o StrictHostKeyChecking=no src/cat/s21_cat src/grep/s21_grep $REMOTE_USER@$REMOTE_HOST:/usr/local/bin

if [ $? -ne 0 ]; then
  echo "Ошибка при копировании файлов"
  exit 1
fi

echo "Файлы успешно скопированы"