<?php 

function filter_sanitize_int_range_generator($low, $high) {
        return function($value = null) use ($low, $high) {
                $v = intval($value);
                if ($v < $low) return false;
                if ($v > $high) return false;
                return $v;
        };
}

function filter_sanitize_string_in_array_generator($array) {
        return function($value = null) use ($array) {
                if (!is_string($value)) return false;
                if (in_array($value, $array)) return $value;
                return false;
        };
}

function coerce_array_values_to_int($array, $keys) {
        foreach($keys as $key) {
                if (isset($array[$key])) {
                        $array[$key] = intval($array[$key]);
                }
        }
        return $array;
}


