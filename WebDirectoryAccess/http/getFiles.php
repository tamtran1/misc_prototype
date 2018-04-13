<?php
	session_start();
	
	$directoryList;
	$fileList;
	$fileSizeList;
	$logData = "";
	
	if (isset ($_SESSION['login'])) {
		if ($_SESSION['login'] != 'tam' && $_POST['directory'] == '/') { //restrict access to root if not user tam
			$_POST['directory'] = '/stuff';
			$_POST['lvl'] = 1;
			exit;
		}
		
		//remove left most '/' from the string if it's not root directory, but keep if this string contains only a '/' 
		$_SESSION['dir'] = strlen($_POST['directory']) > 1 ? substr ($_POST['directory'], 1) : $_POST['directory'];
		$_SESSION['lvl'] = $_POST['lvl'];
		$currDir = "/".$_SESSION['dir'];
		$dir = ".".$currDir;
		$contentArr = scandir($dir);
		
		$logData = $logData.$_SESSION['login']." ".date('m/d/Y h:i:s')." ".$_SERVER['REMOTE_ADDR']." ".$currDir." \r\n";
		$file = fopen("activity.log", "a") or die("Unable to open file!");
		fwrite ($file, $logData);
		fclose($file);
	
		foreach ($contentArr as $item) {	
			if ($item != "." && $item != "..") {
				if (is_dir($dir.DIRECTORY_SEPARATOR.$item)) {
					if (isset ($directoryList))
						$directoryList = $directoryList.$item."|";
					else
						$directoryList = $item."|";
				} else {
					if (isset ($fileList))
						$fileList = $fileList.$item."|";
					else
						$fileList = $item."|";
				
					$bytes = filesize ($dir."/".$item);
				
					if ($bytes >= 1073741824)
						$bytes = number_format($bytes / 1073741824, 2) . ' GB';
					elseif ($bytes >= 1048576)
						$bytes = number_format($bytes / 1048576, 2) . ' MB';
					elseif ($bytes >= 1024)
						$bytes = number_format($bytes / 1024, 2) . ' KB';
					elseif ($bytes > 1)
						$bytes = $bytes . ' bytes';
					elseif ($bytes == 1)
						$bytes = $bytes . ' byte';
					else
						$bytes = '0 bytes';
					
					$fileSizeList = isset ($fileSizeList) ? $fileSizeList.$bytes."|" : $bytes."|";
				}
			}
		}
		
		if (!isset($directoryList))
			$directoryList = "";
		
		if (!isset($fileList)) {
			$fileList = "";
			$fileSizeList = "";
		}	
		
		$dataArr = array("fileList" => $fileList, "fileSizeList" => $fileSizeList, "breadCrumb" => $directoryList);
		print (json_encode($dataArr));
	} else
		print (NULL);
?>