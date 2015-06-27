<?php
/**
 * @file
 */
require_once dirname(__FILE__) . '/../Base.inc';

class VIH_Lectures_Pdf_Portrait_pictures extends VIH_Lectures_Pdf_Base {

  protected $font = 'helvetica';
  protected $frontpage_font = 'helvetica';

  function __construct($events) {
    $this->events = $events;
    parent::__construct('P','mm','A4');
  }

  protected function getPictureFilename($style_name, $uri) {
    $dest = image_style_path($style_name, $uri);
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

  function addFrontpage($image) {
    $this->addPage();
    $this->setY(10);
    $this->SetFont($this->frontpage_font, 'B', 25);
    $this->MultiCell(0, 0, $this->heading, null, 'C');

    $no_of_events = count($this->events);

    $start_x = 15;
    if ($no_of_events >= 6) {

      // edit settings
      $img_size = 42;
      $margen = 3;
      $pictures_pr_row = 4;
      $max_rows = 4;

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
      if (file_exists($image)) {
        $this->Image($image, 10, $this->getY(), 190, 0, '');
      }
      $this->setY($this->getY() + 125);
    }

    $this->setY($this->getY() + 5);
    $this->SetFont($this->frontpage_font, 'B', 30);
    $this->MultiCell(0, 0, $this->sub_title, null, 'C');
    if (!empty($this->logo) && file_exists($this->logo)) {
      $this->Image($this->logo, 60, 240, 90, 0, '', $this->link);
    }
  }

  function addSecondPage() {
    $this->SetFillColor(0, 0, 0);
    $this->SetDrawColor(0, 0, 0);
    $this->addPage();
    $this->SetFont($this->frontpage_font, '', 22);
    $this->MultiCell(0, 0, $this->heading, null, 'L');
    $this->SetFont($this->frontpage_font, 'B', 34);
    $this->Cell(0, 0, $this->sub_title, null, 2, 'L');
    $this->Line(0, 50, 220, 50);
    $this->SetFont($this->frontpage_font, null, 18);
    $this->setTextColor(0, 0, 0);
    $this->setY(60);
    $this->writeHtmlCell(190, 10, $this->GetX(), $this->GetY(), $this->description, 0);
    $this->Line(0, 245, 220, 245);
    if (!empty($this->logo) && file_exists($this->logo)) {
      $this->Image($this->logo, 150, 255, 50, 0, '', $this->link);
    }
  }

  function addEventPage($event) {
    $title = check_plain($event->title);
    $speaker = trim($event->field_speaker[LANGUAGE_NONE][0]['value']);
    $body_field = field_get_items('node', $event, 'body');
    $body = check_markup($this->clearJavascript($body_field[0]['safe_value']), 'filtered_html');
    $picture = field_get_items('node', $event, 'field_picture');
    $location = trim($event->field_location[LANGUAGE_NONE][0]['safe_value']);
    $tilmelding = $this->base_url . '/node/' . $event->nid;
    $start_date = new DateTime($event->field_date[LANGUAGE_NONE][0]['value'], new DateTimeZone($event->field_date[LANGUAGE_NONE][0]['timezone']));
    $end_date = new DateTime($event->field_date[LANGUAGE_NONE][0]['value2'], new DateTimeZone($event->field_date[LANGUAGE_NONE][0]['timezone']));
    $date = ucfirst($this->t($start_date->format('l')) . ', ' . $start_date->format('j.') . $this->t($start_date->format('F')) . ', ' . $start_date->format('Y H:i') . '-' . $end_date->format('H:i'));
    $date = format_date(strtotime($event->field_date[LANGUAGE_NONE][0]['value']) + $start_date->getOffset(), 'custom', 'l, j. F, Y H:i', date_default_timezone_get()) . '-' . format_date(strtotime($event->field_date[LANGUAGE_NONE][0]['value2']) + $end_date->getOffset(), 'custom', 'H:i', date_default_timezone_get());

    // Only works with one product
    if (!empty($event->field_product[LANGUAGE_NONE][0]['product_id'])) {
      $product = commerce_product_load($event->field_product[LANGUAGE_NONE][0]['product_id']);
      $price_calculate = commerce_product_calculate_sell_price($product);
      $price = commerce_currency_format($price_calculate['amount'], $price_calculate['currency_code'], $product);
    }

    $this->addPage();
    $this->setY(10);
    $this->setTextColor(128, 128, 128);
    $this->SetFont($this->font, 'B', 22);
    $this->MultiCell(130, 0, $speaker, null, 'L');
    $this->setTextColor(0, 0, 0);
    $this->SetFont($this->font, 'B', 34);
    $this->MultiCell(130, 0, $title, null, 'L');
    $this->SetFont($this->font, null, 18);
    $this->Line(0, 85, 220, 85);
    $this->setY(95);
    $this->writeHTMLCell(190, 10, $this->GetX(), $this->GetY(), $body);

    $this->Line(0, 245, 220, 245);
    $this->Line(220, 40, 220, 220);

    if ($picture_filename = $this->getPictureFilename('sidepicture', $picture[0]['uri'])) {
      $this->Image($picture_filename, 150, 5, 0, 75, '');
    }

    $qr_file = $this->getBarcodePath($tilmelding, 200, 200);
    if ($qr_file !== false && file_exists($qr_file)) {
      $this->Image($qr_file, 4, 250, 35, 0, '');
    }
    if (!empty($this->logo) && file_exists($this->logo)) {
      $this->Image($this->logo, 151, 260, 50, 0, '', $this->link);
    }
    $x = 40;
    $start_y = 252;
    $line_height = 6;
    $this->setY($start_y);
    $this->setX($x);
    $this->SetFont($this->font, null, 14);
    if (!empty($price)) {
      $this->Write(14, 'Pris: ' . $price);
      $this->setY($start_y + $line_height);
      $this->setX($x);
      $no_lines = 2;
    }
    else {
      $no_lines = 1;
    }
    $this->Write(14, 'Sted: ' . $location);
    $this->setY($start_y + $no_lines * $line_height);
    $this->setX($x);
    $no_lines++;
    $this->Write(14, 'Tid: ' . $date);
    $this->setY($start_y + $no_lines * $line_height);
    $this->setX($x);
    $this->Write(14, 'Tilmelding: ' . $tilmelding);
    $this->Ln();
  }

  /**
   * Implements abstract method
   */
  public function render() {
    $this->SetTitle($this->heading);
    $this->SetSubject($this->sub_title);
    $this->SetAuthor($this->author);
    $this->SetAutoPageBreak(false);
    foreach ($this->events as $event) {
      $this->addEventPage($event);
    }

    $this->Output('foredrag.pdf', 'I');
  }
}
