/* helper http strings and structures
 */
const crumbRecord = (()=>{
	const obj = { //the bread crumb record data object
		"lvl" : 0, //start at lvl zero, the root directory
		"dir" : "/" //the root directory is the directory where this page is located
	};
	return ()=>{ return obj; }
})();

const fStatArr = (()=>{
	const arr = []; //this array holds statistics objects for each file, it is used in the getFiles function
	return ()=>{ return arr; }
})();

const breadCrumbPanelHTML = (()=>{
	const str = "<nav class = \"breadcrumbs\"></nav>";
	return ()=>{ return str; }
})();

const loginHTML = (()=>{
	const	str = "<div id = \"log-in-panel\" class = \"small-6 small-centered large-4 columns\">" +
								"<p></p>" +
								"<div class = \"row column log-in-form\">" +
									"<label>User ID" +
										"<input id = \"userName\" type = \"text\" onkeydown = \"if (event.keyCode == 13) {login();}\">" +
									"</label>" +
									"<label>Password" +
										"<input id = \"password\" type = \"password\" onkeydown = \"if (event.keyCode == 13) login();\">" +
									"</label>" +
									"<p><a style = \"float:right\" href = \"javascript: login();\" class = \"button expanded\">Log In</a></p>" +
									"<err id = \"err\" style = \"color: red\"></err>" +
								"</div>" +
							"</div>";
	return ()=>{ return str; }
})();

const topBarSectionHTML = (()=>{
	const str = "<section id = \"top-bar-section\" class = \"top-bar-section\">" +
								"<ul class = \"right\">" +
									"<li class = \"divider\"></li>" +
									"<li class = \"has-dropdown\">" +
										"<a id = \"id\" href=\"\" disabled></a>" +
										"<ul class = \"dropdown\">" +
											"<li class = \"has-dropdown\">" +
												"<a href=\"\" disabled>Others</a>" +
												"<ul class = \"dropdown\">" +
													"<li><a href = \"index.html\">index</a></li>" +
												"</ul>" +
											"</li>" +
											"<li><a href = \"javascript: logOut();\">Log Out</a></li>" +
										"</ul>" +
									"</li>" +
								"</ul>" +
							"</section>";
	return ()=>{ return str; }
})();

const sessionErr = (()=>{
	const str = "<div style = \"width : 300px\" class = \"small-6 small-centered large-4 columns\"><br>" +
								"<err style =\"color: red\">" +
									"Oops! Can's access this directory.<br>Try to refresh the page or login again.<br><br>" +
									"<a href = \"javascript: login();\" class = \"button [tiny small large]\">Log In Again</a>" +
								"</err>" +
							"</div>";
	return ()=>{ return str; }
})();
	
/* this is the common ajax function, all server requests are invoke through this function
 * takes three parameters, the date to send to the server, the script to which the data is to
 * be handled, and the callback function to whom the data is to be returned. 
 */
ajax = (data, dstScript, callback)=>{
	$.ajax ({
		url: dstScript,	//the data to send
		type: 'POST', //the method used to send the data
		data: data, //the data element
		contentType: "application/x-www-form-urlencoded", //the header
		success: callback //the callback, received data is applied to callback as single object 
	});
}

/* function to clears all the panels, normally used for changing directory or logout
 */
clrPanels = ()=>{
	$('#top-bar-section').html ('');
	$('#bread-crumb-panel').html ('');
	$('#main-content-panel').html ('');
}

/* this function is only called to initialize the top section panel and the breadcrumb
 * then there is log in or refresh is perform
 */
initTopPanels = (id)=>{
	$('#top-bar-section-panel').html (topBarSectionHTML);
	$('#id').html ("Hello " + id);
	$('#bread-crumb-panel').html (breadCrumbPanelHTML);
	$(document).foundation(); // invoke the foundation script. This is important to make the web app work !!
}

/* log out by invoking ajax to destroy session and clears all panels and reloads page
 * this function is called twice, the first call will invoke the ajax since both argument is undefined.
 * this function is also passed as a parameter to perform a callback, this second call clears the panel
 * and reload the page 
 */
logOut = (arg1, arg2)=>{
	if (arg1 && arg2) {
		clrPanels (); //callback returned and session destroy succeeds, clear the panels
		location.reload(); //and reload the page
	} else
		ajax ("logOut=true", "login.php", logOut); //invoke server to destroy the session first
}

/* this check session function is called when the webpage just finish loaded or gets refresh
 * it is invoke twice on each call due to the ajax callback. the first call, both arguments is undefined
 * which will invoke ajax, then the callback from the ajax with the arguments defined. arg1 is the
 * response and arg2 is server response status, both arguments have to be true to determine a started session
 */
checkSession = (arg1, arg2)=>{
	if (arg1 && arg2) {//if call back result in both arg being true then session has started
		$(document).foundation(); //run the foundation script. This line is important to make the web app work !!
		var dataObj = JSON.parse (arg1);
		initTopPanels (dataObj.id); //if session is still active, initialize the top panels again
		chDir(dataObj.lvl, dataObj.dir); //go to designated directory
	} else if (!arg1 && arg2) //call back will results in second argument being true, but if arg1 is null then login require
		login();
	else //if both argument are null, invoke ajax
		ajax ("checkSession=true", "login.php", checkSession);
}

/* this login function is called twice, the first time its called is from the check session function, and the second time is
 * from the ajax callback. the same principle applies similar to the check session, both arguments has to be true to determine
 * a started session
 */
login = (arg1, arg2)=>{
	if ($('#log-in-panel').length) { //first check if div logIn element exist, if not create one
		$('err').html(''); //empty out the error message tag 
		
		if (!$('#userName').val() || !$('#password').val()) { //check if both input fields has a value 
			$('err').html ("Missing fields"); //raise an error message if either fields is empty
			logIn(); //calls itself again, cannot invoke ajax until this if condition is satisfy
		}
		if (arg1 && arg2) { //if both argument are satisfied
			var dataObj = JSON.parse (arg1);
			initTopPanels (dataObj.id); //log in succeed, initialize the top panels
			chDir(dataObj.lvl, dataObj.dir); //go to designated directory
		} else if (!arg1 && arg2) //arg1 is the server response, if this is false, logIn failed raise error message
			$('#err').html ("Log in failed");
		else //if both argument are undefined, invoke ajax to check the username and password
			ajax ("login=true" + "&userName=" + $('#userName').val () + "&password=" + $('#password').val (), "login.php", login);
	} else
		$('#main-content-panel').html(loginHTML()); //populate the fileInfoPanel with the logIn form
}

/* the change directory function clears the panel and updates the crumbRecord struct
 * the crumbRecord struct is responsible for manoeuvrability on the directories structure
 * Note: a browser refresh will zero the crumbRecord.lvl, no need case for lvl == crumbRecord.lvl
 */
chDir = (lvl, dir)=>{	
	if (lvl > crumbRecord().lvl) { //if lvl is greater than the crumbRecord level, then going deeper into sub-folder
		crumbRecord().lvl += lvl - crumbRecord().lvl; //increment a level based on this offset
		crumbRecord().dir += (crumbRecord().dir.length > 1) ? ("/" + dir) : dir; //append the selected directory to the string
		constrBrdCrmb (crumbRecord().lvl); //update the breadcrumb of the new selected folder
	} else {
		crumbRecord().lvl = lvl; //if lvl is lesser than crumbRecord level, then going back out of sub-folder
		constrBrdCrmb(lvl); //update the breadcrumb lvl
	}
}

/* this function takes the an integer argument that represent the directory level
 * and use it to create the directory bread crumb in a loop
 */
constrBrdCrmb = (lvl)=>{
	var dirArr = crumbRecord().dir.split("/"); //split directory string into pieces and puts them in an array
	crumbRecord().dir = "/"; //need to reset directory string, since going up a directory would shortens the string
	//begin constructing the bread crumb HTML, NOTE! first crumb is hard coded, since the '/' was removed after the split
	var breadCrumbHTML = "<nav class = \"breadcrumbs\"><a href = \"javascript: chDir (0, '/');\">/</a>";
	for (var i = 1; i <= lvl; i ++) {
		crumbRecord().dir += (crumbRecord().dir.length > 1) ? ("/" + dirArr[i]) : dirArr[i]; //new directory string reconstruction based on selected bread crumb
			if (i == lvl) //if this is the current directory element, make it non-clickable 
				breadCrumbHTML += "<a class = \"current\">" + dirArr[i] + "</a>";
			else //truncate any long directory name for easier display, if > 10 chars just take the first four and last three chars of string
				breadCrumbHTML += "<a href = \"javascript: chDir (" + i + ", '" + dirArr[i] + "');\">" + ((dirArr[i].length > 10) ? dirArr[i].substr (0, 4) + "..." + dirArr[i].substr (-3) : dirArr[i]) + "</a>";
	}
	breadCrumbHTML += "</nav>"; //attach the closing tags
	
	$('#bread-crumb-panel').html (breadCrumbHTML); //update the new bread crumb HTML content
	//crumbRecord.dir could be an empty string, the '.' indicating root, is appended at the server side
	ajax (data = "directory=" + crumbRecord().dir + "&lvl=" + lvl, "getFiles.php", getFiles);
}

/* this is the get files function, it is invoke as a callback from ajax, which was invoke after constructing the bread crumb
 */
getFiles = (arg)=>{
	if (arg) {
		var dataObj = JSON.parse(arg); //parsing the json object
		var dirArr = dataObj.breadCrumb.split("|");
		dirArr.pop(); //pop off the last empty element that was included after the split
		var fileArr = dataObj.fileList.split("|");
		fileArr.pop(); //pop off the last empty element that was included after the split, this is needed if we are going to iterate 
						//over the list since the last empty element will show up in the browser
		var fSizeArr = dataObj.fileSizeList.split("|"); //parsing the JSON object data into their respective lists
		fStatArr().length = 0; //clear the previous file stat list for content of new directory
		
		var contentListHTML = "<table style=\"border: 0px;\">";
		if (dirArr[0]) //constructing the directory section	
			for (var i in dirArr)
				contentListHTML += "<tr><td><a href = \"javascript: chDir (" + (crumbRecord().lvl + 1) + ", '" + 
					dirArr[i] + "');\">" + dirArr[i] + "</a></td><td>&ltdir&gt</td></tr>";
		
		if (fileArr[0]) //constructing the file section
			for(var i in fileArr) {
				fStatArr().push ({ //pushing the stat of the current file into the list
					name: fileArr[i],
					size: fSizeArr[i],
					loc: crumbRecord().dir + (crumbRecord().dir != "/" ? "/" : "") + fileArr[i]
				});
				
				contentListHTML += "<tr><td><a href = \"" + fStatArr()[i].loc + "\">" + //parameter of location of file
					fStatArr()[i].name + "</a></td><td>" + fStatArr()[i].size + "</td></tr>"; //name of file as it would appear in browser, not a parameter of fileStat
			}
		
		$('#main-content-panel').html (contentListHTML + "</tables>"); //update the fileList with the new HTML content
	} else { //this error situation happens when the user is logged in and then clears their cookies cache while the page is displayed in the browser, 
		clrPanels (); //a refresh or change directory will raise a session error since their credential is missing
		$('#main-content-panel').html (sessionErr); //raises the session error
	}
}

/* this is a self invoking function, this is the first function that gets called when this script gets loaded,
 * as a result this script should be placed at the end html document.
 */
(()=>{ checkSession(); })();