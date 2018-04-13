<html>
  <head>
    <title>PHP Test</title>
  </head>
  <body>
    <?php
      // print ("hello world2 ");
      // $data = fopen ("test.txt", "r") or die ("Unable to open file!");
      // while(!feof($data)) {
      //   $dataContent = explode("|", fgets($data));
      //   print ("<br>".$dataContent[0]." - ".$dataContent[1]);
      // }

      // fclose($data);
      $data = fopen ("test.txt", "r") or die ("Unable to open file!"); //check the login data with data on record, stored in a file
		
      while (!feof ($data)) {
        $dataContent = explode("|", rtrim(fgets($data))); //login is check by exhausting every record entry
        print ("<br>". $dataContent[0]." - ". $dataContent[1]." ");
        if ("temp2" == $dataContent[0] && "temp3" == $dataContent[1]) {
          print ("match");
          fclose ($data);
          exit;
        }
      }
      fclose($data);
    ?> 
  </body>
</html>