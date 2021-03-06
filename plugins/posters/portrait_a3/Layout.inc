<?php
/**
 * @file
 */

require_once dirname(__FILE__) . '/../Base.inc';

class VIH_Lectures_Pdf_Poster_Portrait_a3 extends VIH_Lectures_Pdf_Poster_Base {

  protected $font = 'dejavusans';
  protected $frontpage_font = 'dejavusans';
  protected $image;
  protected $uri = 'vih.dk/foredrag';

  function __construct($events, $image = null) {
    $this->events = $events;
    $this->image = $image;
    parent::__construct('P','mm','A3');
  }

  protected function getPictureFilename($style_name, $uri) {
    $dest = image_style_path($stylename, $uri);
    if (!file_exists($dest)) {
      $style = image_style_load($style_name);      
      image_style_create_derivative($style, $uri, $dest);
    }
    $picture_filename = drupal_realpath($dest);
    if (!file_exists($picture_filename)) {
      return FALSE;
    }
    return $picture_filename;
  }

  function addHeader() {
    $this->addPage();
    $this->setY(10);
    $this->SetFont($this->frontpage_font, 'B', 25);
    $this->MultiCell(0, 0, $this->heading, null, 'C');

    $no_of_events = count($this->events);

    $start_x = 15;
    if ($no_of_events >= 6) {

      // edit settings
      $img_size = 48;
      $margen = 4;
      $pictures_pr_row = 5;
      $max_rows = 3;

      $row_count = 0;
      $x = $start_x;
      $y = $this->getY() + 5;
      $i = 0;
      $j = 1;

      $rows = $no_of_events / $pictures_pr_row;
      if (floor($rows) > $max_rows) {
        $rows = $max_rows;
      }

      foreach ($this->events as $event) {
        $picture = field_get_items('node', $event, 'field_picture');
        if ($picture_filename = $this->getPictureFilename('panopoly_image_square', $picture[0]['uri'])) {
          $this->Image($picture_filename, $x, $y, $img_size, 0, '');
        }
        $x += $img_size + $margen;
        if ($j >= $pictures_pr_row) {
          $y += $img_size + $margen;
          $x = $start_x;
          $j = 0;
          $row_count++;
        }
        $i++;
        $j++;
        if ($row_count > $rows) {
          break;
        }
      }

      $missing_squares = $pictures_pr_row - $j + 1;

      for ($u = 0; $u < $missing_squares; $u++){
        $r = rand(0, 255);
        $s = rand(0, 255);
        $t = rand(0, 255);
        $this->SetFillColor($r, $s, $t);
        $this->SetDrawColor($r, $s, $t);
        $this->Rect($x, $y, $img_size, $img_size, 'DF');
        $x += $img_size + $margen;
      }

      if ($i <= $pictures_pr_row) {
        $this->setY($this->getY() + $img_size + 5);
      }
      elseif ($rows != floor($rows)) {
        $this->setY($y + $img_size + 5);
      }
      else {
        $this->setY($y);
      }
    }
    else {
      $this->setY($this->getY() + 5);
      if (file_exists($this->image)) {
        $this->Image($this->image, 10, $this->getY(), 190, 0, '');
      }
      $this->setY($this->getY() + 125);
    }

    $this->setY($this->getY() + 5);
    $this->SetFont($this->frontpage_font, 'B', 30);
    $this->MultiCell(0, 0, $this->sub_title, null, 'C');
    if (!empty($this->logo) && file_exists($this->logo)) {
      $this->Image($this->logo, 195, 370, 80, 0, '', $this->link);
    }

    if (file_exists(dirname(__FILE__) . '/fof-logo.jpg')) {
      $this->Image(dirname(__FILE__) . '/fof-logo.jpg', 10, 370, 50, 0, '', 'http://fof-vejle.dk');
    }
    $this->setY($this->getY() + 10);
    /*
    $qr_file = $this->getBarcodePath('http://' . $this->uri, 200, 200);
    if ($qr_file !== false && file_exists($qr_file)) {
      $this->Image($qr_file, 4, 250, 35, 0, '');
    }
    */ 
  }

  function addEvent($event) {
    $title = check_plain($event->title);
    $speaker = trim($event->field_speaker[LANGUAGE_NONE][0]['value']);
    $start_date = new DateTime($event->field_date[LANGUAGE_NONE][0]['value'], new DateTimeZone($event->field_date[LANGUAGE_NONE][0]['timezone']));
    $end_date = new DateTime($event->field_date[LANGUAGE_NONE][0]['value2'], new DateTimeZone($event->field_date[LANGUAGE_NONE][0]['timezone']));
    $date = ucfirst($this->t($start_date->format('l')) . ', ' . $start_date->format('j.') . $this->t($start_date->format('F')) . ', ' . $start_date->format('Y H:i') . '-' . $end_date->format('H:i'));
    $date = format_date(strtotime($event->field_date[LANGUAGE_NONE][0]['value']) + $start_date->getOffset(), 'custom', 'j/m, H:i', date_default_timezone_get());

    $this->SetFont($this->font, 'N', 18);
    $this->setY($this->getY());
    $this->MultiCell(270, 0, $date . ': ' . $speaker . ' - ' . $title, null, 'L');
  }

  /**
   * Implements abstract method
   */
  public function render() {
    $this->SetTitle($this->heading);
    $this->SetSubject($this->sub_title);
    $this->SetAuthor($this->author);
    $this->SetAutoPageBreak(false);
    $this->addHeader();
    foreach ($this->events as $event) {
      $this->addEvent($event);
    }

    $this->Output('foredrag.pdf', 'I');
  }
}
