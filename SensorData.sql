DROP TABLE IF EXISTS SensorData;
CREATE TABLE `SensorData` (
  `id` int(6) unsigned NOT NULL AUTO_INCREMENT,
  `sensor` varchar(30) COLLATE utf8_unicode_ci NOT NULL,
  `location` varchar(30) COLLATE utf8_unicode_ci NOT NULL,
  `value1` varchar(10) COLLATE utf8_unicode_ci DEFAULT NULL,
  `value2` varchar(10) COLLATE utf8_unicode_ci DEFAULT NULL,
  `value3` varchar(10) COLLATE utf8_unicode_ci DEFAULT NULL,
  `value4` varchar(10) COLLATE utf8_unicode_ci DEFAULT NULL,
  `value5` varchar(10) COLLATE utf8_unicode_ci DEFAULT NULL,
  `reading_time` timestamp NOT NULL DEFAULT current_timestamp() ON UPDATE current_timestamp(),
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=10 DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;



INSERT INTO SensorData(id,sensor,location,value1,value2,value3,value4,value5,reading_time) VALUES(1,'14702613','37.8718992,-122.2585399','21.00','63.00','1015.00','-65','2.99','2021-01-30 04:55:45'),(2,'14702613','37.8718992,-122.2585399','22.00','50.00','1044.00','-63','2.99','2021-01-30 04:58:02'),(3,'14702613','37.8718992,-122.2585399','21.00','53.00','1074.00','-55','2.99','2021-01-30 04:59:05'),(4,'14702613','37.8718992,-122.2585399','25.00','57.00','1005.00','-55','2.99','2021-01-30 05:00:09'),(5,'14702613','37.8718992,-122.2585399','27.00','89.00','1032.00','-53','2.99','2021-01-30 05:01:12'),(6,'14702613','37.8718992,-122.2585399','26.00','64.00','1026.00','-49','2.99','2021-01-30 05:02:16'),(7,'14702613','37.8718992,-122.2585399','23.00','85.00','1002.00','-50','2.99','2021-01-30 05:03:20'),(8,'14702613','37.8718992,-122.2585399','20.00','71.00','1044.00','-50','2.99','2021-01-30 05:04:24'),(9,'14702613','37.8718992,-122.2585399','29.00','76.00','1052.00','-50','2.99','2021-01-30 05:05:29');







