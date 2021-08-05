<?php
function _header($content) {
    return implode("", $content[0]);
}
function _content($file) {return array_map(function($line) {
    return explode(";", trim($line));
}, explode("\n", file_get_contents($file)));}

if(!$argv[1]) {
    echo "Specify project directory";
    return;
}

$dir = $argv[1];
$content = _content("{$dir}/1.map");
$reference = _header($content);
$result = array_fill(0, count($content) - 3, array_fill(0, count($content[3]), []));

$files = "{$dir}/*.map";
foreach(glob($files) as $file) {
    if(strrpos($file, "target.map") > 0) continue;
    echo $file . PHP_EOL;
    $content = _content($file);

    // Correct map
    if(_header($content) == $reference) {
        $i = 0;
        $lines = array_slice($content, 3);
        foreach($lines as $line) {
            $j = 0;
            foreach($line as $item) {
                if($item == 'nan' || is_numeric($item)) {
                    $result[$i][$j++][] = $item;
                }
            }
            $i++;
        }
    }
}

function processMatrix($result) {
    $resultMean = [];
    foreach($result as $k => $line) {
        $resultMean[$k] = [];
        foreach($line as $values) {
            if($values) {
                $values = array_filter($values, function($v) {return $v != 'nan';});
                // Среднее значение
                $m = 0; $cnt = 0;
                foreach($values as $value) {
                    $m += $value;
                    $cnt++;
                }
                if($cnt > 0) {
                    $m /= $cnt;
                    $resultMean[$k][] = $m;
                } else {
                    $resultMean[$k][] = 0;
                }
            }
        }
    }

    $resultUncertainty = [];

    foreach($result as $k => $line) {
        $resultUncertainty[$k] = [];
        $j = 0;
        foreach($line as $values) {
            if($values) {
                $values = array_filter($values, function($v) {return $v != 'nan';});
                if(count($values) < 2) {
                    $resultUncertainty[$k][] = 'nan';
                    continue;
                }
                // Среднее значение
                $m = $resultMean[$k][$j++];

                // Экспериментальная дисперсия
                $s = 0; $cnt = 0;
                foreach($values as $value) {
                    $s += pow($value - $m, 2);
                    $cnt++;
                }
                $s /= $cnt - 1;

                // Статистическая оценка дисперсии
                $s2 = $s / $cnt;

                // Разброс
                $u = sqrt($s2);

                $resultUncertainty[$k][] = round($u, 4);
            }
        }
    }

    $resultDispersion = [];
    foreach($result as $k => $line) {
        $resultDispersion[$k] = [];
        $j = 0;
        foreach($line as $values) {
            if($values) {
                $values = array_filter($values, function($v) {return $v != 'nan';});
                // Среднее значение
                $m = $resultMean[$k][$j];

                // Экспериментальная дисперсия
                $s = 0;
                foreach($values as $value) {
                    $s += pow($value - $m, 2);
                }
                $s /= $cnt - 1;

                $resultDispersion[$k][$j++] = round($s, 4);
            }
        }
    }

    return [$resultMean, $resultUncertainty, $resultDispersion];
}

list($resultMean, $resultUncertainty, $resultDispersion) = processMatrix($result);

$maxTolerance = 0.01;
foreach($resultUncertainty as $i => $row) {
    foreach($row as $j => $column) {
        if($column > $maxTolerance) {
            echo "\e[31m" . $column . "\t";
        } else {
            echo "\e[39m" . $column . "\t";
        }
    }
    echo PHP_EOL;
}
foreach($resultMean as $i => $row) {
    foreach($row as $j => $column) {
        $column = round($column, 4);
        if($resultUncertainty[$i][$j] > $maxTolerance) {
            echo "\e[31m" . $column . "\t";
        } else {
            echo "\e[39m" . $column . "\t";
        }
    }
    echo PHP_EOL;
}

$getAround = function($row, $column) use(&$result, $resultUncertainty, $maxTolerance, $resultMean, $resultDispersion) {
    $values = 0; $cnt = 0;
    for ($i = max($row - 1, 0); $i < min($row + 1, count($result)); $i++) {
        for ($j = max($column - 1, 0); $j < min($column + 1, count($result[$i])); $j++) {
            // Reliable value
            if($resultUncertainty[$i][$j] < $maxTolerance) {
                $values += $resultMean[$i][$j];$cnt++;
            }
        }
    }
    if($cnt > 0) {
        $mean = $values / $cnt;
        $s = 2 * sqrt($resultDispersion[$row][$column]);
        foreach($result[$row][$column] as $k => $value) {
            if(($value > ($mean - $s)) && ($value < $mean + $s)) {
                // ok
            } else {
                $result[$row][$column][$k] = 'nan';
            }
        }
    }
};
$c = 0;
for($row = 0; $row < count($result); $row++) {
    if(count($resultUncertainty[$row]) > 0) {
        for ($column = 0; $column < count($result[$row]); $column++) {
            if ($resultUncertainty[$row][$column] > $maxTolerance) {
                echo $getAround($row, $column);
            }
        }
    }
}

function printColumn($row) {
    foreach ($row as $item) {
        foreach ($item as $v) {
            echo round($v, 4) . " ";
        }
        echo PHP_EOL;
    }
}
list($resultMean, $resultUncertainty, $resultDispersion) = processMatrix($result);

$resultMap = implode("\n", array_map(function($l) {return implode(";", $l);}, [$content[0], $content[1], $content[2]]));
foreach($resultMean as $i => $row) {
    $resultMap .= "\n" . implode(";", array_map(function($x) {return round($x, 4);}, $row));
    foreach($row as $j => $column) {
        $column = round($column, 4);
        if($resultUncertainty[$i][$j] > $maxTolerance) {
            echo "\e[31m" . $column . "\t";
        } else {
            echo "\e[39m" . $column . "\t";
        }
    }
    echo PHP_EOL;
}
echo "\e[39m";

$target = "{$dir}/target.map";
file_put_contents($target, $resultMap);
