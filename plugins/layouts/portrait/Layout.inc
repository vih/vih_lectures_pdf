<?php
/**
 * @file
 */
require_once dirname(__FILE__) . '/../PortraitLayout.inc';

class VIH_Lectures_Pdf_Portrait extends VIH_Lectures_Pdf_PortraitLayout {

  function addFrontpage($image) {
    $this->addPage();
    $this->Image($image, 10, 10, 190, 0, '');
    $this->setY(140);
    $this->SetFont($this->frontpage_font, 'B', 50);
    $this->MultiCell(0, 0, $this->heading, null, 'C');
    $this->SetFont($this->frontpage_font, 'B', 40);
    $this->MultiCell(0, 0, $this->sub_title, null, 'C');
    if (!empty($this->logo) && file_exists($this->logo)) {
      $this->Image($this->logo, 55, 230, 100, 0, '', 'http://vih.dk/');
    }
  }

  function addSecondPage() {
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
      $this->Image($this->logo, 150, 255, 50, 0, '', 'http://vih.dk/');
    }
  }

  function addEventPage($event) {
    $title = check_plain($event->title);
    $speaker = trim($event->field_speaker[LANGUAGE_NONE][0]['value']);
    $body_field = field_get_items('node', $event, 'body');
    $body = check_markup($this->clearJavascript($body_field[0]['value']), 'panopoly_wysiwyg_text');
    $picture = field_get_items('node', $event, 'field_picture');
    $picture_filename = drupal_realpath(image_style_path('sidepicture', $picture[0]['uri']));
    $location = trim($event->field_location[LANGUAGE_NONE][0]['safe_value']);
    $price = trim($event->field_price[LANGUAGE_NONE][0]['value']);
    $tilmelding = 'http://vih.dk/node/' . $event->nid;
    $start_date = new DateTime($event->field_date[LANGUAGE_NONE][0]['value'], new DateTimeZone($event->field_date[LANGUAGE_NONE][0]['timezone']));
    $end_date = new DateTime($event->field_date[LANGUAGE_NONE][0]['value2'], new DateTimeZone($event->field_date[LANGUAGE_NONE][0]['timezone']));
    $date = ucfirst($this->t($start_date->format('l')) . ', ' . $start_date->format('j.') . $this->t($start_date->format('F')) . ', ' . $start_date->format('Y H:i') . '-' . $end_date->format('H:i'));
    $date = format_date(strtotime($event->field_date[LANGUAGE_NONE][0]['value']) + $start_date->getOffset(), 'custom', 'l, j. F, Y H:i', date_default_timezone_get()) . '-' . format_date(strtotime($event->field_date[LANGUAGE_NONE][0]['value2']) + $end_date->getOffset(), 'custom', 'H:i', date_default_timezone_get());

    $this->addPage();
    $this->SetFont($this->font, 'B', 22);
    $this->setY(10);
    $this->Cell(150, 0, $speaker, null, 2, 'L');
    $this->SetFont($this->font, 'B', 34);
    $this->MultiCell(130, 0, $title, null, 'L');
    $this->SetFont($this->font, null, 18);
    $this->setTextColor(0, 0, 0);
    $this->Line(0, 85, 220, 85);
    $this->setY(95);
    $this->SetFont($this->font, null, 14);
    $this->writeHTMLCell(190, 10, $this->GetX(), $this->GetY(), $body);

    // Make sure pointer set to new page if one.
    $this->lastPage();
    // Set y to last cell height, doesn't include passed $h for some reason.
    $this->setY($this->lasth+20);

    $this->Line(0, $this->GetY(), 220, $this->GetY());
    $this->Line(220, 40, 220, 220);

    // Offset bottom a tad.
    $this->setY($this->GetY() + 5);

    if ($picture_filename) {
      $this->Image($picture_filename, 150, 5, 0, 75, '');
    }

    $qr_file = $this->getBarcodePath($tilmelding, 200, 200);
    if ($qr_file !== false) {
      $this->Image($qr_file, 4, $this->GetY(), 35, 0, '');
    }
    if (!empty($this->logo) && file_exists($this->logo)) {
      $this->Image($this->logo, 150, $this->GetY() + 5, 50, 0, '', 'http://vih.dk/');
    }
    $x = 40;
    $this->setX($x);
    $this->SetFont($this->font, null, 14);
    $this->Write(14, 'Pris: ' . $price . ' kroner');
    $this->setY($this->GetY() + 6);
    $this->setX($x);
    $this->Write(14, 'Sted: ' . $location);
    $this->setY($this->GetY() + 6);
    $this->setX($x);
    $this->Write(14, 'Tid: ' . $date);
    $this->setY($this->GetY() + 6);
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
    $this->SetAuthor('Vejle Idrætshøjskole');
    $this->SetAutoPageBreak(TRUE, 0);
    foreach ($this->events as $event) {
      $this->addEventPage($event);
    }


    $this->Output('foredrag.pdf', 'I');
  }


}
