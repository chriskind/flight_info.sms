<?php

class flightinfoSMS {
  
  function __construct() {

  }
  

  function check_sms_enabled($phone_num = NULL, $message = NULL, $command = NULL) {
    $output = array("SMS Messaging is disabled\r\n"  . $command . "\r\n" . $phone_num . "\r\n" . $message);
    return $output;
  }
  
  function test_sms($phone_num=NULL, $subject=NULL, $message=NULL) {
    
    $phone_num = variable_get('SMS_Test_Phone');
    $message = variable_get('site_name', "Default site name") . ' system test';
    $subject = variable_get('site_name', "Default site name") . ' system test';
    
    $output = $this->send_sms($phone_num, $subject, $message);
    return $output;
  }
  
  function send_sms($phone_num, $subject, $message, $auth_info = NULL, $test = NULL) {
    
    if(variable_get('SMS_Test_Mode') == 1) {
      $phone_num = variable_get('SMS_Test_Phone');
    }
    
    $script = variable_get('SMS_Script_Path');
    $script .= '/aws_sms.sh';
    
    // $command = "aws --profile snssys sns publish --phone-number \"$phone_num\" --subject \"$subject\" --message \"$message\" 2>&1";
    $command = "$script -p \"$phone_num\" -s \"$subject\" -m \"$message\" 2>&1"; 
    
    $output = array();
    
    if(variable_get('SMS_Enabled', 0) == 0) {
      $output = $this->check_sms_enabled($phone_num, $message, $command);
      return $output;
    }
    
    exec($command, $output);
    
    // aws_sms.sh results
    $error = stripos($output, 'error');
    
    // user friendly message for content admins
    if(stripos($output[1], 'error') > 0) {
      drupal_set_message($phone_num . ' message failed to send', 'error');
      watchdog('flightinfo_sms', 'AWS SMS failure', $output);
    } else {
      drupal_set_message($phone_num . ' successfully submitted');
    }
    
    if(function_exists('adhd_messages_allowed')) {
      $debug = adhd_messages_allowed();
      
      if($debug === TRUE) {
        adhd_buffer_to_dsm($output, 'dpm');
        adhd_buffer_to_dsm("command from php:\r\n$command", 'dpm');
        // $output[] = $command;
      }
    }
    
    return $output;
  }
  
}   


