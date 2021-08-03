<?php
$file = $argv[1] ?? die("No file");
$content = file_get_contents($file);
file_put_contents($file, preg_replace("/fill:rgb\(\d+,\d+,\d+\)/", "fill:rgb(0,0,0)", $content));
