<?php  
require_once libraries_get_path('tcpdf') . '/tcpdf.php';

abstract class VIH_Lectures_Pdf_Base extends TCPDF {
  protected $heading = 'Vejle Idrætshøjskoles foredrag';
  protected $sub_title = 'Kom og høre med!';
  protected $description = 'description';
  protected $logo;
  protected $events;

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

  /**
   * @todo Should utilize Drupal own t()-function
   */
  protected function t($phrase) {
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
  
  abstract public function render();
  
  /**
   * Gets barcode file path
   *
   * @param string $url Registration url
   * @param integer $height Height
   * @param integer $width Width
   *
   * @return string or false
   */
  protected function getBarcodePath($url, $height, $width) {
    $chart = array(
      '#chart_id' => 'test_chart',
      '#type' => CHART_TYPE_QR,
      '#size' => chart_size(200, 200) 
    );
        
    $chart['#data'][] = '';
    $chart['#labels'][] = $url;
        
    return chart_copy($chart, 'my_chart_' . uniqid(), 'public://charts/');
  }
}
