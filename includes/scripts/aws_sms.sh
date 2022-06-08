#!/bin/bash

#
# ***  THE SERVER TIMEZONE HAS TO BE SET TO THE AWS SNS API TIME! ***
#

phone_num='+16786991683'
subject='flight delay'
message='sms test'

# use flags for options when calling script aws_sms.sh -p 12345678910 -s the subject -m the message
while getopts ":p:s:m:" o; do
  case "${o}" in
    p)
      phone_num=${OPTARG}
      ;;
    s)
      subject=${OPTARG}
      ;;
    m)
      message=${OPTARG}
      ;;
  esac
done

# echo $phone_num
# echo $subject
# echo $message

# load credentials & config
export AWS_ACCESS_KEY_ID=AKIAIYCNGCYJY7DLKH4A
export AWS_SECRET_ACCESS_KEY=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

# aws binary location local:/usr/local/bin/ 
# aws binary location bitnami:/usr/local/aws/bin

# LOCAL COMMAND (COMMENT OUT FOR DEV AND PROD)
/usr/local/bin/aws sns publish --phone-number "$phone_num" --subject "$subject" --message "$message" --region us-east-1 --output text

# DEV, STAGE, PROD COMMAND (COMMENT OUT FOR DEV AND PROD)
# /usr/local/aws/bin/aws sns publish --phone-number "$phone_num" --subject "$subject" --message "$message" --region us-east-1 --output text