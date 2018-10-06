<?php // vim:et:ai:ts=4:sw=4
namespace LA;

class Tools 
{
    /**
     * Usage: \LA\Tools::arrayToPDOParameters($ar);
     */
    static function arrayToPDOParameters($ar)
    {
        $b = [];
        foreach($ar as $key=>$value) {
            $b[':'.$key] = $value;
        }
        return $b;
    }

    /**
     * Ad hoc date library to deal with dates from the database.
     * Dates are stored as GMT, but without that ISO format because this is MySQL :(
     * The old queries adjusted the dates and returned dates and formatted dates.
     * The new system just takes the GMT date from the database, and doesn't
     * decorate with the fancy dates.  We use these functions to do that old work.
     * 
     * Usage:
     *
     * \LA\Tools::month($this->article['created']);
     * \LA\Tools::year($this->article['created']);
     */

    static function setupDateStuff() 
    {
        global $time_zone;
        global $time_diff;

        if (!isset($time_zone)) $time_zone=date('Z'); // timezone offset in seconds
        if (!isset($time_diff)) $time_diff=0;
        return [$time_zone, $time_diff];
    }

    static function activeDateToDateTime($date) 
    {
        list($tzo, $td) = self::setupDateStuff();
        $dt = DateTime($date);
        if ($tzo >= 0) {
            $dt->modify("+$tzo seconds");
        } else {
            $dt->modify("$tzo seconds");
        }
        return $td;
    }

    static function month($date)
    {
        self::setupDateStuff();
        $dt = self::activeDateToDateTime($date);
        return $dt->format('MM');
    }

    static function year($date)
    {
        self::setupDateStuff();
        $dt = self::activeDateToDateTime($date);
        return $dt->format('YY');
    }
}
