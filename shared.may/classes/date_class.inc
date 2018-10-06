<?php

class Date
{
     //PHP has no native Date function that can
     //do things like moving forward a day. This class represents a date object
     //and should have all common functions related to manipulation of dates.

    var $timestamp = 0;

    function get_hour()
    {
        //returns the hour portion of the date object
        $datearray=getdate($this->timestamp);
        return $datearray["hours"];
    }

    function get_minute()
    {
        //returns the minute portion of the date object
        $datearray=getdate($this->timestamp);
        return $datearray["minutes"];
    }

    function get_day()
    {
       //returns the day portion of the date object
        $datearray=getdate($this->timestamp);
        return $datearray["mday"];
    }

    function get_month()
    {
        //returns the month portion of the date object
        $datearray=getdate($this->timestamp);
        return $datearray["mon"];
    }

    function get_year()
    {
        //returns the year portion of the dat object
        $datearray=getdate($this->timestamp);
        return $datearray["year"];
    }

    function set_time_zone()
    {
        // sets the time based on the system offset, then the local offset
        $this->set_date_to_now();
        $offset = $GLOBALS['time_diff'] - $GLOBALS['server_time_offset'];
        $this->timestamp+=($offset*3600);
    }

    function get_formatted_date()
    {
        $form_date = date("d/m/Y", $this->timestamp);
        return $form_date;
    }

    function get_formatted_time()
    {
        $form_time = date("h:i A", $this->timestamp);
        return $form_time;
    }

    function get_day_of_week()
    {
        //Returns the text name of the day of the week ("Mon", "Tue" etc..)
        if (date("D", $this->timestamp)=="Sun")
        {
            return 0;
        } else if (date("D", $this->timestamp)=="Mon")
        {
            return 1;
        } else if (date("D", $this->timestamp)=="Tue")
        {
            return 2;
        } else if (date("D", $this->timestamp)=="Wed")
        {
            return 3;
        } else if (date("D", $this->timestamp)=="Thu")
        {
            return 4;
        } else if (date("D", $this->timestamp)=="Fri")
        {
            return 5;
        } else if (date("D", $this->timestamp)=="Sat")
        {
            return 6;
        } else
        {
            return "error";
        }
    }

    function get_weekname()
    {
        //returns the date of the beginning of the week
        $newdate=$this->aclone();
        $newdate->find_start_of_week();
        $return_string=$newdate->get_year();
        $return_string=$return_string."_";
        $return_string=$return_string.$newdate->get_month();
        $return_string=$return_string."_";
        $return_string=$return_string.$newdate->get_day();
        return $return_string;
    }

    function set_date_to_now()
    {
        //sets this date object to point to teh current date
        $this->timestamp=time();
    }

    function find_start_of_week()
    {
        //moves the date object to the beginning of the week (compared to its current date)
        $this->move_to_start_of_day();
        $this->move_to_start_of_day();
        while ($this->get_day_of_week()>0)
        {
            $this->move_forward_n_days(-1);
        }
    }

    function move_to_start_of_day()
    {
        //moves a date object to teh beginning of the day that it currently represents
        $hour=0;
        $minute=0;
        $second=0;
        $day=$this->get_day();
        $month=$this->get_month();
        $year=$this->get_year();
        $newtimestamp=mktime( $hour, $minute, $second, $day, $month, $year);
    }

    function move_forward_n_days($n)
    {
        //moves the date object forward N days (can be negative to go backwards)
        $hour=$this->get_hour();
        $minute=$this->get_minute();
        $second=0;
        $day=$this->get_day()+$n;
        $month=$this->get_month();
        $year=$this->get_year();
        $newtimestamp=mktime( $hour, $minute, $second, $month,$day, $year);
        $this->timestamp=$newtimestamp;
    }

    function move_n_weeks_forward($n)
    {
        //moves teh date object forward N weeks (can be negative to go backwards)
        $this->move_forward_n_days(7*$n);
    }

    function aclone() // jhh 5.7.09 -- renamed from "function clone()" to aclone()
    {
        //returns a new Date object given an old one
        $newdate= new Date;
        $newdate->timestamp=$this->timestamp;
        return $newdate;
    }

    function get_sql_date()
    {
        //returns a date in the format needed for inserts by MySQL
        $year=$this->get_year();
        $month=$this->get_month();
        if (strlen($month)<2)
        {
            $month="0".$month;
        }

        $day=$this->get_day();

        if (strlen($day)<2)
        {
            $day="0".$day;
        }
        return $year."-".$month."-".$day." 00:00:00";
    }

    function get_sql_time()
    {
        //not yet written
    }

    function get_unix_time()
    {
        //retuns the Unix timestamp of the given date
        return $this->timestamp;
    }

    function set_time_from_sql_time($sqltime)
    {
        //not yet written
    }

    function set_time_from_unix_time($unixtime)
    {
        //sets the datetime of the date object based off a unix time stamp
        $this->timestamp=$unixtime;
    }

    function set_time_from_string($string_time)
    {
        //not yet written
    }

    function set_time($newhour, $newminute, $newday, $newmonth, $newyear)
    {
	//echo "creating date: $newhour, $newminute, $newday, $newmonth, $newyear <br>";
        //sets the Date object given the hour, minute, day, month, and  year
        $newtimestamp=mktime( $newhour, $newminute, 0, $newmonth, $newday, $newyear);
        $this->timestamp=$newtimestamp;
    }

}

?>
