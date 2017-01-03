<?php

class Time
{
    //class representing a time 
    //this class should contain all methods for manipulating and rendering times
    var $time = 0;

    function set_time($newtime)
    {
        //sets a time as a number between 0 and 24
        $this->time=$newtime;
    }

    function get_time()
    {
        //retuns the time as a number between 0 and 24
        return $this->time;
    }

    function get_hour()
    {
        //returns the hour portion of the time object in 24 hour format
        return floor($this->time);
    }

    function get_minutes()
    {
        //returms the minute portion of the time object
        return round(60*($this->time-$this->get_hour()));
    }

    function get_is_am()
    {
        //returns true if the time is in the AM
        $hour = $this->get_hour();
        $test = floor($hour/12);
        $test = $test % 2;
        if ($test == 0)
        {
            return true;
        } else
        {
            return false;
        }
    }

    function get_am()
    {
        //returns "AM" or "PM" depending on if the time is in the AM or PM
        if ($this->get_is_am())
        {
            return "AM";
        } else
        {
            return "PM";
        }
    }

    function get_12hour()
    {
        //returns the hour hour portion of the time object in 12 hour format
        $hour = $this->get_hour();
        if ($hour < 0) return 0;
        if (!(($hour-1000) < 0)) return 0;
        if ($hour == 0) return 12;
        while ($hour > 12)
        {
            $hour = $hour - 12;
        } 
        return $hour;
    }

    function get_display_time()
    {
        //returns a formatted time in 12 hour format
        $time = $this->get_12hour();
        $time = $time . ":";
        $minute = $this->get_minutes();
        if ($minute < 10)
        {
            $minute = "0" . $minute;
        }
        $time = $time . $minute . " ";
        $time = $time . $this->get_am();
        return $time;
    }

}
?>