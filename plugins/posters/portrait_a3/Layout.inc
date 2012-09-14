<?php
/**
 * @file
 */
require_once dirname(__FILE__) . '/../Base.inc';

class VIH_Lectures_Pdf_Poster_Portrait_a3 extends VIH_Lectures_Pdf_Poster_Base {

  protected $font = 'dejavusans';
  protected $frontpage_font = 'dejavusans';
  protected $image;

  function __construct($events, $image = null) {
    $this->events = $events;
    $this->image = $image;
    parent::__construct('P','mm','A3');
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
      $img_size = 60;
      $pictures_pr_row = 4;
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
        $picture_filename = drupal_realpath(image_style_path('square_thumbnail', $picture[0]['uri']));
        if (file_exists($picture_filename)) {
          $this->Image($picture_filename, $x, $y, $img_size, 0, '');
        }
        $x += $img_size;
        if ($j >= $pictures_pr_row) {
          $y += $img_size;
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
      $this->Image($this->logo, 55, 240, 90, 0, '', $this->link);
    }
  }

  function addEvent($event) {
    $title = check_plain($event->title);
    $speaker = trim($event->field_speaker['und'][0]['value']);
    $body_field = field_get_items('node', $event, 'body');
    $body = check_markup($this->clearJavascript($body_field[0]['safe_value']), 'filtered_html');
    $picture = field_get_items('node', $event, 'field_picture');
    $picture_filename = drupal_realpath(image_style_path('sidepicture', $picture[0]['uri']));
    $location = trim($event->field_location['und'][0]['safe_value']);
    $price = trim($event->field_price['und'][0]['value']);
    $tilmelding = $this->base_url . '/node/' . $event->nid;
    $start_date = new DateTime($event->field_date['und'][0]['value'], new DateTimeZone($event->field_date['und'][0]['timezone']));
    $end_date = new DateTime($event->field_date['und'][0]['value2'], new DateTimeZone($event->field_date['und'][0]['timezone']));
    $date = ucfirst($this->t($start_date->format('l')) . ', ' . $start_date->format('j.') . $this->t($start_date->format('F')) . ', ' . $start_date->format('Y H:i') . '-' . $end_date->format('H:i'));
    $date = format_date(strtotime($event->field_date['und'][0]['value']) + $start_date->getOffset(), 'custom', 'l, j. F, Y H:i', date_default_timezone_get()) . '-' . format_date(strtotime($event->field_date['und'][0]['value2']) + $end_date->getOffset(), 'custom', 'H:i', date_default_timezone_get());

    $this->SetFont($this->font, 'B', 22);
    $this->setY($this->getY());
    $this->Cell(150, 0, $speaker, null, 2, 'L');
    $this->SetFont($this->font, 'B', 34);
    $this->MultiCell(130, 0, $title, null, 'L');
    $this->SetFont($this->font, null, 18);
    $this->setTextColor(0, 0, 0);

    $qr_file = $this->getBarcodePath('http://vih.dk/foredrag', 200, 200);
    if ($qr_file !== false && file_exists($qr_file)) {
      $this->Image($qr_file, 4, 250, 35, 0, '');
    }
    if (!empty($this->logo) && file_exists($this->logo)) {
      $this->Image($this->logo, 150, 255, 50, 0, '', $this->link);
    }
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