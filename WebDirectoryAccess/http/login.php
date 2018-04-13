<?php	
	session_start(); //start the session

	/*======================< create new user section >======================*/
	//to create a new account, just uncomment this area and enter in a user name and password, 
	//and refresh the client's browser, and a new account will be created
	// $data = fopen("test.dat", "a") or die("Unable to open file!"); //uncomment this line when creating new account

	// $cost = 10; // the bigger the cost the better the password will be after hashed
	// $number = strtr(base64_encode($iv), '+', '.'); // will create a random numbers
	// $number = sprintf("$2a$%02d$", $cost) . $number; // prefix the password for the compare log in later
	
	// fwrite ($data, crypt("test1", $number)."|".crypt("test1", $number)."|\n"); //put the user name and password in the according position 
	// exit;
	/*======================< end of crate new user section >======================*/
	
	if (isset ($_POST['logOut'])) {
		session_destroy(); //destroy all session when logged output
		print (true); //return true on success
		exit;
	}
	
	if (isset ($_POST['checkSession'])) {
		if (isset ($_SESSION['login'])) { //check if session is set
			$dataArr = array ("id" => $_SESSION['login'], "dir" => $_SESSION['dir'], "lvl" => $_SESSION['lvl']); //if true, get its previous directory state
			print (json_encode ($dataArr)); //encode the directory state into json object and send it back to client
		} else
			print (null); //respond null if no session is set
		exit;
	}
	
	if (isset ($_POST['login'])) {
		$data = fopen ("tmp.dat", "r") or die ("Unable to open file!"); //check the login data with data on record, stored in a file
		
		while (!feof ($data)) {
			$dataContent = explode("|", rtrim(fgets($data))); //login is check by exhausting every record entry 
			// if ($dataContent[0] && password_verify($_POST['userName'], $dataContent[0]) && password_verify($_POST['password'], $dataContent[1])) {
			if ($_POST['userName'] == $dataContent[0] && $_POST['password'] == $dataContent[1]) {
				$_SESSION['login'] = $_POST['userName']; //creates a session for this user, and keep track of their activity
				$_SESSION['dir'] = 'stuff'; //subdirectory relative to the directory containing the main index.html file 
				$_SESSION['lvl'] = 1; //1 directory deep, directory containing index.html is main directory, or lvl 0 
				if (!file_exists ('stuff')) { mkdir ('stuff', 0777); } // create this directory if it doesn't exist

				$dataArr = array ("id" => $_SESSION['login'], "dir" => $_SESSION['dir'], "lvl" => $_SESSION['lvl']); //construct the list of data
				$dataArr = array ("id" => $_SESSION['login'], "dir" => $_SESSION['dir'], "lvl" => $_SESSION['lvl']); //construct the list of data				
				print (json_encode ($dataArr)); //encode it into a json object and send it back to the client
				fclose ($data);
				exit;
			}
		}
		
		session_destroy(); //if all record entries are exhausted and login failed, destroy session to free memory
		fclose($data); //clode the file handler to free memory
		print (NULL);
	}	
?>