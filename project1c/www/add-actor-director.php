<!DOCTYPE HTML>
<html>
        <head>
            <link type="text/css" rel="stylesheet" href="style/style.css">
            <link href='http://fonts.googleapis.com/css?family=Lato&subset=latin,latin-ext' rel='stylesheet' type='text/css'>
            <title>Project 1C: Movie Website</title>
        </head>
        <body>
            <div id="navicol">
                <a href="index.php"><div id="logo">Project 1C</div></a>
                <a href="add-actor-director.php"><div class="navibutton">Add Actor/Director</div></a>
                <a href="add-movie.php"><div class="navibutton">Add Movie Info</div></a>
		<a href="add-review.php"><div class="navibutton">Add Review</div></a>
                <a href="relate-movie-actor.php"><div class="navibutton">Relate Movie/Actor</div></a>
                <a href="relate-movie-director.php"><div class="navibutton">Relate Movie/Director</div></a>
                <a href="search.php"><div class="navibutton">Search Actors/Movies</div></a>
            </div>

            <div id="content">
                <div id="raised-interface">
                    <h1> Add New Actor/Director </h1>
                    <form method="GET">
                        <h3> Actor/Director </h3>
                        <select name="person-type">
                            <option value="Actor">Actor</option>
                            <option value="Director">Director</option>
                        </select>

                        <h3> First Name </h3>
                        <input type="text" placeholder="first name" name="first-name"/>            
                        <h3> Last Name </h3>
                        <input type="text" placeholder="last name" name="last-name"/>            
                        <h3> Gender </h3>
                        <select name="gender">
                            <option value="Male">Male</option>
                            <option value="Female">Female</option>
                        </select>

                        <h3> Date of Birth </h3>
                        <input type="text" placeholder="i.e. 1995-10-28" name="date-of-birth"/> 
                        
                        <h3> Date of Death </h3>
                        <input type="text" placeholder="date of death (blank if still alive)" name="date-of-death"/>

                        <br>
                        <br>
                        <input type="submit" value="Submit"/>                   
                    </form>
                    <div id="response">
        <?php
            // checks a string to see if valid mysql date
            function check_input_year($date) {
                $strlen = strlen($date);
                if($strlen != 10) {
                    return false;
                }                
                
                for($i = 0; $i < $strlen; $i++) {
                    $char = $date[$i];
                    // if any of the first 4 chars are not numeric
                    if ($i < 4 and !is_numeric($char)) {
                        return false;
                    }
                    else if ($i == 4 and $char != '-') {
                        return false;
                    }
                    else if ($i > 4 and $i < 7 and !is_numeric($char)) {
                        return false;
                    }
                    else if ($i == 7 and $char != '-') {
                        return false;
                    }
                    else if ($i > 7 and !is_numeric($char)) {
                        return false;
                    }
                }
                $year = intval(substr($date, 0, 4));
                $month = intval(substr($date, 5, 2));
                $day = intval(substr($date, 8, 2));                
    
                // if invalid gregorian date
                if (!checkdate($month, $day, $year)) {
                    return false;
                }
    
                return true;
            }            

            $servername = "localhost";
            $username = "cs143";
            $password = "";
            $dbname = "CS143";
            
            // Create connection
            $conn = new mysqli($servername, $username, $password, $dbname);
            
            // Check connection
            if ($conn->connect_error) {
                die("Connection failed: " . $conn->connect_error);
            } 
            $correct_formatting = true;
    
            // get query from text-area
            $forms = $_GET;
            if (empty($forms)) {
                $correct_formatting = false;
            }        
            // check that all fields are not empty except dod
            foreach($forms as $key => $value) {
                if ($key != "date-of-death" and $value == "") {
                    echo $key . " field is blank!<br>";
                    $correct_formatting = false;
                }
                // check date formatting
                if (($key == "date-of-birth" or ($key == "date-of-death" and $value != "")) and !check_input_year($value)) {
                    echo "Date formatting problem!<br>";
                    $correct_formatting = false;
                }
            }                        
            if ($correct_formatting) {            
                // get the max id
                $max_id_res = $conn->query("SELECT * FROM MaxPersonID");
                $max_id = $max_id_res->fetch_assoc()["id"];
                
                if ($forms["date-of-death"] == "") {
                    $dod = "0000-00-00";
                } else {
                    $dod = $forms["date-of-death"];
                }

                if ($forms["person-type"] == "Actor") {
                    $query = "INSERT INTO Actor VALUES (" . $max_id . ", '" . $forms["last-name"] . "', '" . $forms["first-name"] . "', '" . $forms["gender"] . "', '" . $forms["date-of-birth"] . "', '" . $dod . "');";
                    echo $query;
                } else {
                    $query = "INSERT INTO Director VALUES (" . $max_id . ", '" . $forms["last-name"] . "', '" . $forms["first-name"] . "', '" . $forms["date-of-birth"] . "', '" . $dod . "');";
                }
                $result = $conn->query($query);
                 
                if (!$result) {
                    echo "Invalid Query!";
                } else {
                    // if all went well, update the max person id
                    $update_query = "UPDATE MaxPersonID SET id=id+1;";
                    $conn->query($update_query);
                    echo "Added Successfully!";
                    
                }
            }
            $conn->close();
        ?>
                </div>
                <br class="clear"/>
                </div>
            </div>

        </body>
</html>
