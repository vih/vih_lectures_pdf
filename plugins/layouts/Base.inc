<?php  
require_once libraries_get_path('tcpdf') . '/tcpdf.php';

abstract class VIH_Lectures_Pdf_Base extends TCPDF {
  protected $heading = 'Vejle Idrætshøjskoles foredrag';
  protected $sub_title = 'Kom og høre med!';
  protected $description = 'description';
  protected $logo;
  protected $events;

  function __construct($events) {
    $this->events = $events;
  }

  /**
   * @param string $logo Path to the logo
   */  
  function setLogo($logo) {
    $this->logo = $logo;
  }
  
  function setHeading($title) {
    $this->heading = $title;
  }

  function setSubtitle($title) {
    $this->sub_title = $title;
  }

  function setDescription($desc) {
    $this->description = $desc;
  }
    
  function Header() {
    // left blank to remove line in header added by TCPDF
  }
    
  function Footer() {
    // left blank to remove line in header added by TCPDF
  }
    
  function clearJavascript($s) {
    $do = true;
    while ($do) {
      $start = stripos($s, '<script');
      $stop = stripos($s, '</script>');
      if ((is_numeric($start)) && (is_numeric($stop))) {
        $s = substr($s, 0, $start) . substr($s, ($stop + strlen('</script>')));
      } 
      else {
        $do = false;
      }
    }
    return trim($s);
  }
  
  abstract public function render();
}
