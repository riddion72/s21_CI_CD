#!bin/bash

TELEGRAM_BOT_TOKEN="6704730737:AAG0guGaKN40xFqRsjSKwDA-T9YWkOO6Er8"
TELEGRAM_USER_ID="470701656"

sleep 5

TIME="10"
URL="https://api.telegram.org/bot$TELEGRAM_BOT_TOKEN/sendMessage"

# if [ "$CI_JOB_STATUS" == "success" ]; then
#   TEXT="$1 ✅ %0A%0AProject:+$CI_PROJECT_NAME%0AURL:+$CI_PROJECT_URL/pipelines/$CI_PIPELINE_ID/"
# else
#   TEXT="$1 🚫 %0A%0AProject:+$CI_PROJECT_NAME%0AURL:+$CI_PROJECT_URL/pipelines/$CI_PIPELINE_ID/"
# fi

TEXT="$1: $CI_JOB_STATUS. %0A%0AProject:+$CI_PROJECT_NAME%0AURL:+$CI_PROJECT_URL/pipelines/$CI_PIPELINE_ID/"


curl -s --max-time $TIME -d "chat_id=$TELEGRAM_USER_ID&disable_web_page_preview=1&text=$TEXT" $URL > /dev/null