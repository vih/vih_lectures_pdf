<?php
abstract class VIH_Lectures_Pdf_Base extends TCPDF {
    protected $pdf;
    protected $heading = 'Vejle Idrætshøjskoles foredrag';
    protected $sub_title = 'Kom og høre med!';
    protected $description = 'description';
    protected $logo;
    
    function setHeading($title)
    {
        $this->heading = $title;
    }

    function setSubtitle($title)
    {
        $this->sub_title = $title;
    }

    function setDescription($desc)
    {
        $this->description = $desc;
    }
    
    function Header()
    {
        // left blank to remove line in header added by TCPDF
    }
    
    function Footer()
    {
        // left blank to remove line in header added by TCPDF
    }
    
    function clearJavascript($s) {
        $do = true;
        while ($do) {
            $start = stripos($s,'<script');
            $stop = stripos($s,'</script>');
            if ((is_numeric($start))&&(is_numeric($stop))) {
                $s = substr($s,0,$start).substr($s,($stop+strlen('</script>')));
            } else {
                $do = false;
            }
        }
        return trim($s);
    }
    
    /**
     * @todo Should utilize Drupal own t()-function
     */
    function t($phrase)
    {
        // Default to English
        $phrases = array(
          'Monday' => 'mandag',
          'Tuesday' => 'tirsdag',
          'Wednesday' => 'onsdag',
          'Thursday' => 'torsdag',
          'Friday' => 'fredag',
          'Saturday' => 'lørdag',
          'Sunday' => 'søndag',
          'January' => 'januar',
          'February' => 'februar',
          'March' => 'marts',
          'April' => 'april',
          'May' => 'maj',
          'June' => 'juni',
          'July' => 'juli',
          'August' => 'august',
          'September' => 'september',
          'October' => 'oktober',
          'November' => 'november',
          'December' => 'december',
          '1' => 'januar',
          '2' => 'februar',
          '3' => 'marts',
          '4' => 'april',
          '5' => 'maj',
          '6' => 'juni',
          '7' => 'juli',
          '8' => 'august',
          '9' => 'september',
          '10' => 'oktober',
          '11' => 'november',
          '12' => 'december'
        );
        if (!empty($phrases[$phrase])) {
            return $phrases[$phrase];
        }
        return $phrase;
    }
}
