<?php
require 'vendor/autoload.php';

use opency\SQLiteConnection;

$pdo = (new SQLiteConnection())->connect();

/* try {
   $this->pdo = new \PDO("sqlite:" . Config::PATH_TO_SQLITE_FILE);
} catch (\PDOException $e) {
   // handle the exception here
} */

if ($pdo != null)
    echo 'Connected to the SQLite database successfully!';
else
    echo 'Could not connect to the SQLite database!';