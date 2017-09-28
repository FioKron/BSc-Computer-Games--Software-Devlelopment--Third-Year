<?php
	$db = mysql_connect('localhost:3306', 'Test', 'test')	or die ('Could not connect: ' . mysql_error());
	mysql_select_db('TestDB') or die ('Could not select database');
	
	// 'Strings must be escaped to prevent SQL injection attack.'
	$Email = mysql_real_escape_string($_GET['Email'], $db);
	
?>