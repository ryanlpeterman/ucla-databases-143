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
                        <select name="gender">
                            <option value="Male">Male</option>
                            <option value="Female">Female</option>
                        </select>
                        <h3> Date of Birth </h3>
                        <input type="text" placeholder="date of birth" name="date-of-birth"/> 
                        <h4> ie: 1995-10-28 </h4>
                        <h3> Date of Death </h3>
                        <input type="text" placeholder="date of death" name="date-of-death"/>
                        <h4> (Leave blank if still alive) </h4> 

                        <br>
                        <br>
                        <input type="submit" value="Submit"/>                   
                    </form>
                </div>
            </div>
        </body>
</html>
