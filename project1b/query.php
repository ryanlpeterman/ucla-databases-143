<html>
    <head> 
        <title> CS143: Project 1B </title> 
        <link href="https://fonts.googleapis.com/css?family=Roboto" rel="stylesheet">
        <style>
            body {
                margin-top: 40px;
                background-color: #edeff2;
            }
            table, th, td {
                border: 1px solid black;
                margin: auto;
                background-color: white;
                box-shadow: 0 0 10px #888888;    
            }
            #query-tile {
                background-color: white;
                padding: 15px;
                box-shadow: 0 0 10px #888888;    
                width: 800px;
                margin: auto;
                margin-bottom: 50px;
            }
            #form-container {
                width: 500;
                height: auto;
                margin:auto; 
                display: block; 
                margin-bottom: 40px; 
            }
            * {
                font-family: 'Roboto', sans-serif;
                text-align: center;
            }
            textarea{
                margin-top: 20px;
                margin-bottom: 20px;
            }
            input {
                display: block;
                margin:auto;
                height: 30px;
                width: 80px;
            }
        </style>
    </head>
    <body>
        <div id="query-tile">
            <h1> Web Interface for Database Query </h1>
            <p> Max Chern - 304290221 <br> Ryan Peterman - 704269982</p>

            <div id="form-container">
                   <form method="GET">
                        <textarea rows="8" cols="60" name="input-query"></textarea>
                        <br>    
                        <input type="submit" value="Submit">
                   </form>
            </div>
        </div>
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

            // get query from text-area
            $query = $_GET["input-query"];
            $result = $conn->query($query);
            
            if (!$result) {
                echo "Invalid Query!";
            } else {

                // holds the header of the table 
                $column_header = "<table><tr>";
                // holds fields for easy access later
                $header_fields = array();
                
                foreach (($result->fetch_fields()) as $value) {
                        // for every column we append to html table
                        $column_header = $column_header . "<th>". $value->name . "</th>";
                        array_push($header_fields, $value->name);
                }
                // close header
                $column_header = $column_header . "</tr>";
                echo $column_header;
                
                if ($result->num_rows > 0) {
                     // output data of each row
                     while($row = $result->fetch_assoc()) {
                            echo "<tr>";
                             foreach ($header_fields as $header) {
                                echo "<td>" . $row[$header] . "</td>";
                             }     
                            echo "</tr>";
                     }
                     echo "</table>";
                }  
            }
            
            $conn->close();
        ?>
    </body>
</html>
