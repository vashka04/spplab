<?php
function readScores(array $scores): array {
    $result = ['sum' => 0, 'min' => null, 'max' => null, 'valid' => 0];
    foreach ($scores as $score) {
        if (!is_numeric($score)) {
            continue;
        }
        $score = (float)$score;
        if ($score < 0 || $score > 100) {
            continue;
        }
        $result['sum'] += $score;
        $result['min'] = $result['min'] === null ? $score : min($result['min'], $score);
        $result['max'] = $result['max'] === null ? $score : max($result['max'], $score);
        $result['valid']++;
    }
    return $result;
}
function calculateAverage(float $sum, int $count): float {
    return $count > 0 ? $sum / $count : 0.0;
}
function getGrade(float $average): string {
    if ($average >= 90) {
        return 'A';
    } elseif ($average >= 75) {
        return 'B';
    } elseif ($average >= 60) {
        return 'C';
    } elseif ($average >= 50) {
        return 'D';
    }
    return 'F';
}
function countAbove(array $scores, float $limit): int {
    $count = 0;
    foreach ($scores as $score) {
        if (is_numeric($score) && $score >= $limit) {
            $count++;
        }
    }
    return $count;
}
function buildReport(array $stats, float $average): array {
    $grade = getGrade($average);
    $spread = ($stats['max'] ?? 0) - ($stats['min'] ?? 0);
    $level = 'low';
    switch ($grade) {
        case 'A':
        case 'B':
            $level = 'high';
            break;
        case 'C':
            $level = 'medium';
            break;
        default:
            $level = 'low';
    }
    return [
        'average' => round($average, 2),
        'grade' => $grade,
        'level' => $level,
        'spread' => round($spread, 2)
    ];
}
$scores = [95, 82, 76, 64, 91, 58, 73, 88, 47, 100, 69, 84];
$stats = readScores($scores);
$average = calculateAverage($stats['sum'], $stats['valid']);
$report = buildReport($stats, $average);
$above70 = countAbove($scores, 70);
$above90 = countAbove($scores, 90);
echo "=== SCORE ANALYSIS ===\n";
echo "Valid: " . $stats['valid'] . ", Sum: " . $stats['sum'] . "\n";
echo "Min: " . $stats['min'] . ", Max: " . $stats['max'] . "\n";
echo "Average: " . $report['average'] . ", Grade: " . $report['grade'] . "\n";
echo "Level: " . $report['level'] . ", Spread: " . $report['spread'] . "\n";
echo "Scores >=70: " . $above70 . ", >=90: " . $above90 . "\n";
$groups = ['excellent' => 0, 'good' => 0, 'other' => 0];
foreach ($scores as $score) {
    if ($score >= 90) {
        $groups['excellent']++;
    } elseif ($score >= 70) {
        $groups['good']++;
    } else {
        $groups['other']++;
    }
}
echo "Groups: ";
foreach ($groups as $name => $amount) {
    echo strtoupper($name) . "=" . $amount . " ";
}
echo "\n";
$index = 0;
while ($index < count($scores)) {
    $current = $scores[$index];
    $status = ($current >= 80 && $current <= 100) ? 'strong' : 'regular';
    if ($index % 2 === 0) {
        echo "Item " . ($index + 1) . ": " . $current . " (" . $status . ")\n";
    }
    $index++;
}
$weighted = 0.0;
for ($i = 0; $i < count($scores); $i++) {
    $weight = ($i + 1) / count($scores);
    $weighted += $scores[$i] * $weight;
}
$count = count($scores);
$weighted = round($weighted / (($count * ($count + 1) / 2) / $count), 2);
echo "Weighted indicator: " . $weighted . "\n";
if ($report['grade'] === 'A' && $above90 > 1) {
    echo "Result: excellent performance.\n";
} elseif ($report['grade'] === 'F' || $report['average'] < 50) {
    echo "Result: additional practice is required.\n";
} else {
    echo "Result: regular practice is recommended.\n";
}
?>