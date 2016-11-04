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
                <a href="relate-movie-actor.php"><div class="navibutton">Relate Movie/Actor</div></a>
                <a href="relate-movie-director.php"><div class="navibutton">Relate Movie/Director</div></a>
                <a href="find-actor.php"><div class="navibutton">Find Actor</div></a>
                <a href="find-movie.php"><div class="navibutton">Find Movie</div></a>
                <a href="search.php"><div class="navibutton">Search Actors/Movies</div></a>
            </div>

            <div id="content">
                <div id="raised-interface">
                    <h1> Search Actors and Movies </h1>
                    <form id="searchform" method="GET">
                        <h3> Search </h3>
                        <input type="text" placeholder="query" name="search"/>            
                        <br>
                        <br>
                        <input type="submit" value="Submit"/>                   


                    <?php
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
                    $forms = $_GET;
                    if (!empty($forms)) {
                        $search_words = $parts = preg_split('/\s+/', $forms["search"]);
                        
                        $like_actor = "WHERE ";
                        $like_movie = "WHERE ";
                        foreach ($search_words as $word) {
                            $like_movie = $like_movie . "title LIKE '%" . $word ."%' AND ";
                            $like_actor = $like_actor . "CONCAT(first, ' ', last) LIKE '%" . $word . "%' AND ";
                        }
                        
                        // remove the last AND and add a semicolon
                        $like_movie = substr($like_movie, 0, -5) . ";";
                        $like_actor = substr($like_actor, 0, -5) . ";";

                        
                        $movie_query = "SELECT * FROM Movie " . $like_movie;
                        $actor_query = "SELECT * FROM Actor " . $like_actor;
                        
                        $movie_res = $conn->query($movie_query);
                        $actor_res = $conn->query($actor_query);
                        
                        function echo_mysql_res($result) {

                            // holds the header of the table 
                            $column_header = "<table><tr>";
                            // holds fields for easy access later
                            $header_fields = array();
                            
                            foreach (($result->fetch_fields()) as $value) {
                                    // for every column we append to html table
                                    if ($value->name == "last" or $value->name == "first" or $value->name == "title" or $value->name == "dob" or $value->name == "year") {
                                        $column_header = $column_header . "<th>". $value->name . "</th>";
                                        array_push($header_fields, $value->name);
                                    }
                            }
                            // close header
                            $column_header = $column_header . "</tr>";
                            echo $column_header;
                            
                            if ($result->num_rows > 0) {
                                 // output data of each row
                                 while($row = $result->fetch_assoc()) {
                                        echo "<tr>";
                                         foreach ($header_fields as $header) {
                                            if($header == "last" or $header == "first" or $header == "dob") {
                                                echo "<td><a href='show.php?aid=" . $row["id"] . "'>" . $row[$header] . "</a></td>";
                                            } else {
                                
                                                echo "<td><a href='show.php?mid=" . $row["id"] . "'>" . $row[$header] . "</a></td>";
                                            } 
                                        }     
                                        echo "</tr>";
                                 }
                                 echo "</table>";
                            }  
                        }
                        
                        if (!$movie_res or !$actor_res) {
                            echo "Invalid Query";
                        } else {
                            echo "<h3> Actor Results </h3>";
                            echo_mysql_res($actor_res);
            
                            echo "<h3> Movie Results </h3>";
                            echo_mysql_res($movie_res);
                        }
                    }
                    $conn->close();
                    ?>
                    </form>
                    <br class="clear"/>
                </div>
            </div>
        </body>
</html>
