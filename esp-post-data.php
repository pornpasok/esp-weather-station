<?php
  include_once('esp-database.php');

  // Keep this API Key value to be compatible with the ESP code provided in the project page. If you change this value, the ESP sketch needs to match
  $api_key_value = "********";

  $api_key= $sensor = $location = $value1 = $value2 = $value3 = $value4 = "";

  if ($_SERVER["REQUEST_METHOD"] == "POST") {
    $api_key = test_input($_POST["api_key"]);
    if($api_key == $api_key_value) {
      $sensor = test_input($_POST["sensor"]);
      $location = test_input($_POST["location"]);
      $value1 = test_input($_POST["value1"]);
      $value2 = test_input($_POST["value2"]);
      $value3 = test_input($_POST["value3"]);
      $value4 = test_input($_POST["value4"]);

      $result = insertReading($sensor, $location, $value1, $value2, $value3, $value4);
      echo $result;
    }
    else {
      echo "Wrong API Key provided.";
    }
  }
  else {
    echo "No data posted with HTTP POST.";
  }

  function test_input($data) {
    $data = trim($data);
    $data = stripslashes($data);
    $data = htmlspecialchars($data);
    return $data;
  }