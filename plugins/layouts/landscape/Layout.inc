<?php
/**
 * @file
 */
require_once dirname(__FILE__) . '/../Base.inc';

class VIH_Lectures_Pdf_Landscape extends VIH_Lectures_Pdf_Base {

  function __construct($events) {
    $this->events = $events;
    parent::__construct('L','mm','A4');
  }

  function addFrontpage($image) {
    $this->addPage();

    if (!empty($this->logo) && file_exists($this->logo)) {
      $this->Image($this->logo, 100, 55, 100, 0, '', 'http://vih.dk/');
    }
    $this->Image($image, 10, 10, 277,0,'');
    $this->SetFont('Helvetica', 'B', 35);
    $this->setTextColor(255, 255, 255);
    $this->setY(15);
    $this->Cell(0, 35, $this->heading, null, 2, 'C', true);
  }

  function addSecondPage() {
    $this->addPage();
    $this->SetFont('Helvetica', 'B', 26);
    $this->setTextColor(255, 255, 255);
    $this->Cell(0, 22, $this->heading, null, 2, 'C', true);
    $this->SetFont('Helvetica', null, 18);
    $this->setTextColor(0, 0, 0);
    $this->setY(40);
    $this->writeHtmlCell(275, 10, $this->GetX(), $this->GetY(), $this->description, 0);
    if (!empty($this->logo) && file_exists($this->logo)) {
      $this->Image($this->logo, 235, 165, 50, 0, '', 'http://vih.dk/');
    }
  }

  function addEventPage($event) {
    $title = check_plain($event->title);
    $body_field = field_get_items('node', $event, 'body');
    $body = check_markup($this->clearJavascript($body_field[0]['value']), 'panopoly_wysiwyg_text');
    $picture = field_get_items('node', $event, 'field_picture');
    $filename = drupal_realpath($picture[0]['uri']);
    $location = trim($event->field_location[LANGUAGE_NONE][0]['safe_value']);
    $price = trim($event->field_price[LANGUAGE_NONE][0]['value']);
    $start_date = new DateTime($event->field_date[LANGUAGE_NONE][0]['value'], new DateTimeZone($event->field_date[LANGUAGE_NONE][0]['timezone']));
    $end_date = new DateTime($event->field_date[LANGUAGE_NONE][0]['value2'], new DateTimeZone($event->field_date[LANGUAGE_NONE][0]['timezone']));
    $date = ucfirst($this->t($start_date->format('l')) . ', ' . $start_date->format('j.') . $this->t($start_date->format('F')) . ', ' . $start_date->format('Y H:i') . '-' . $end_date->format('H:i'));
    $date = format_date(strtotime($event->field_date[LANGUAGE_NONE][0]['value']) + $start_date->getOffset(), 'custom', 'l, j. F, Y H:i', date_default_timezone_get()) . '-' . format_date(strtotime($event->field_date[LANGUAGE_NONE][0]['value2']) + $end_date->getOffset(), 'custom', 'H:i', date_default_timezone_get());

    $this->addPage();
    $this->SetFont('Helvetica', 'B', 26);
    $this->setTextColor(255, 255, 255);
    $this->Cell(0, 22, $title, null, 2, 'C', true);
    $this->SetFont('Helvetica', null, 18);
    $this->setTextColor(0, 0, 0);
    $this->setY(40);
    $this->writeHTMLCell(200, 10, $this->GetX(), $this->GetY(), $body);

    $this->Line(10, 165, 220, 165);
    $this->Line(220, 40, 220, 220);

    if ($filename) {
      $this->Image($filename, 225, 40, 60, 0, '');
    }

    $qr_file = $this->getBarcodePath($tilmelding, 200, 200);

    if ($qr_file !== false) {
      $this->Image($qr_file, 235, 135, 45, 0, '');
    }

    if (!empty($this->logo) && file_exists($this->logo)) {
      $this->Image($this->logo, 235, 180, 50, 0, '', 'http://vih.dk/');
    }
    $this->setY(168);
    $this->SetFont('Helvetica', null, 14);

    $this->MultiCell(200, 12, 'Pris og sted: ' . $price . ' kroner, ' . $location, 0, 'L');

    $this->setY(174);

    $this->Write(14, 'Tid: ' . $date);
    $this->Ln();
  }

  function render() {
    $this->SetTitle($this->heading);
    $this->SetSubject($this->sub_title);
    $this->SetAuthor('Vejle Idrætshøjskole');
    $this->SetAutoPageBreak(false);
    foreach ($this->events as $event) {
      $this->addEventPage($event);
    }

    $this->Output('foredrag.pdf', 'I');
  }
}
