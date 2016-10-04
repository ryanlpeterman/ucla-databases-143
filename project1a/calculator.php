<html>
        <head><title>Project 1A: Calculator</title></head>

        <body>
                <h1>CS143 Project 1A: Calculator</h1>
                <p> By: Max Chern (304290221) and Ryan Peterman (704269982) </p>

                <p>
                        <form method="GET">
                                <input type="text" name="expr">
                                <input type="submit" value="Calculate">
                        </form>
                </p>

        </body>

        <p> Result: </p>
        <?php
                $regex = "/^(\s*)([\-]?[0-9]+([\.]([0-9]+))?)(([\s]*[\+\-\/\*][\s]*([\-]?[0-9]+([\.]([0-9]+))?)))*(\s*)/";

                // validate the input
                if (preg_match($regex, $_GET["expr"], $matches) and strlen($_GET["expr"])==strlen($matches[0])) {
                        // print the expression with spaces removed
                        $display = preg_replace('/\s+/', '', $_GET["expr"]);
                        echo $display . " = ";

                        // echo the string after evaluated
                        eval("echo ".$_GET["expr"].";");
                }
                else {
                        echo "Invalid Expression!!";
                }
        ?>

</html>

