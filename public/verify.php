<?php

# verifies the configuration
# prints a report

include('shared/global.cfg');

function ok($var) {
	$value = $GLOBALS[$var];
	echo "<p>$var: $value</p>";
}
function bad($var, $message) {
	$value = $GLOBALS[$var];
	echo "<p>ERROR: $var $message: $value</p>";
}
function okc($var) {
	$value = get_defined_constants()[$var];
	echo "<p>$var: $value</p>";
}
function badc($var, $message) {
	$value = get_defined_constants()[$var];
	echo "<p>ERROR: $var $message: $value</p>";
}

if (file_exists($server_root)) { ok('server_root'); } else { bad('server_root', 'does not exist'); }
if (file_exists($config_dir)) { ok('config_dir'); } else { bad('config_dir', 'does not exist'); }
if (file_exists(SF_SHARED_PATH)) { okc('SF_SHARED_PATH'); } else { badc('SF_SHARED_PATH', 'does not exist'); }
if (filter_var(SF_ADMIN_URL, FILTER_VALIDATE_URL)) { okc('SF_ADMIN_URL'); } else { badc('SF_ADMIN_URL', 'invalid url'); }
if (file_exists($archive_cache_path)) { ok('archive_cache_path'); } else { bad('archive_cache_path', 'does not exist'); }
if (file_exists($imconvert_path)) { ok('imconvert_path'); } else { bad('imconvert_path', 'does not exist'); }

