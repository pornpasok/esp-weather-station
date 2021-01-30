<?php
  // REPLACE with your Host name
  $servername = "HOSTNAME";
  // REPLACE with your Database name
  $dbname = "DBNAME";
  // REPLACE with Database user
  $username = "USERNAME";
  // REPLACE with Database user password
  $password = "PASSWORD";

  function insertReading($sensor, $location, $value1, $value2, $value3, $value4, $value5) {
    global $servername, $username, $password, $dbname;

    // Create connection
    $conn = new mysqli($servername, $username, $password, $dbname);
    // Check connection
    if ($conn->connect_error) {
      die("Connection failed: " . $conn->connect_error);
    }

    $sql = "INSERT INTO SensorData (sensor, location, value1, value2, value3, value4, value5)
    VALUES ('" . $sensor . "', '" . $location . "', '" . $value1 . "', '" . $value2 . "', '" .$value3 . "', '" . $value4 . "', '" . $value5 . "')";

    if ($conn->query($sql) === TRUE) {
      return "New record created successfully";
    }
    else {
      return "Error: " . $sql . "<br>" . $conn->error;
    }
    $conn->close();
  }
  
  function getAllReadings($limit) {
    global $servername, $username, $password, $dbname;

    // Create connection
    $conn = new mysqli($servername, $username, $password, $dbname);
    // Check connection
    if ($conn->connect_error) {
      die("Connection failed: " . $conn->connect_error);
    }

    $sql = "SELECT id, sensor, location, value1, value2, value3, value4, value5, reading_time FROM SensorData order by reading_time desc limit " . $limit;
    if ($result = $conn->query($sql)) {
      return $result;
    }
    else {
      return false;
    }
    $conn->close();
  }

  // Gen Graph
  function getAllGraph($limit) {
    global $servername, $username, $password, $dbname;

    // Create connection
    $conn = new mysqli($servername, $username, $password, $dbname);
    // Check connection
    if ($conn->connect_error) {
      die("Connection failed: " . $conn->connect_error);
    }

    $sql = "SELECT id, sensor, location, value1, value2, value3, value4, value5, reading_time FROM SensorData order by reading_time desc limit " . $limit;
    if ($result = $conn->query($sql)) {
      return $result;
    }
    else {
      return false;
    }
    $conn->close();
  }

  function getLastReadings() {
    global $servername, $username, $password, $dbname;

    // Create connection
    $conn = new mysqli($servername, $username, $password, $dbname);
    // Check connection
    if ($conn->connect_error) {
      die("Connection failed: " . $conn->connect_error);
    }

    $sql = "SELECT id, sensor, location, value1, value2, value3, value4, value5, reading_time FROM SensorData order by reading_time desc limit 1" ;
    if ($result = $conn->query($sql)) {
      return $result->fetch_assoc();
    }
    else {
      return false;
    }
    $conn->close();
  }

  function minReading($limit, $value) {
     global $servername, $username, $password, $dbname;

    // Create connection
    $conn = new mysqli($servername, $username, $password, $dbname);
    // Check connection
    if ($conn->connect_error) {
      die("Connection failed: " . $conn->connect_error);
    }

    $sql = "SELECT MIN(" . $value . ") AS min_amount FROM (SELECT " . $value . " FROM SensorData order by reading_time desc limit " . $limit . ") AS min";
    if ($result = $conn->query($sql)) {
      return $result->fetch_assoc();
    }
    else {
      return false;
    }
    $conn->close();
  }

  function maxReading($limit, $value) {
     global $servername, $username, $password, $dbname;

    // Create connection
    $conn = new mysqli($servername, $username, $password, $dbname);
    // Check connection
    if ($conn->connect_error) {
      die("Connection failed: " . $conn->connect_error);
    }

    $sql = "SELECT MAX(" . $value . ") AS max_amount FROM (SELECT " . $value . " FROM SensorData order by reading_time desc limit " . $limit . ") AS max";
    if ($result = $conn->query($sql)) {
      return $result->fetch_assoc();
    }
    else {
      return false;
    }
    $conn->close();
  }

  function avgReading($limit, $value) {
     global $servername, $username, $password, $dbname;

    // Create connection
    $conn = new mysqli($servername, $username, $password, $dbname);
    // Check connection
    if ($conn->connect_error) {
      die("Connection failed: " . $conn->connect_error);
    }

    $sql = "SELECT AVG(" . $value . ") AS avg_amount FROM (SELECT " . $value . " FROM SensorData order by reading_time desc limit " . $limit . ") AS avg";
    if ($result = $conn->query($sql)) {
      return $result->fetch_assoc();
    }
    else {
      return false;
    }
    $conn->close();
  }
?>