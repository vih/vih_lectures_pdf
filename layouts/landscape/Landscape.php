<?php
/**
Title - should be a specific node
Subtitle - should be a specific node
Frontpage - picture - should be a node
Lectures could be attached by entity reference

Pdf of single lectures
Pdf of all lectures in a view
Pdf of specific lectures with frontpage etc
**/

class VIH_Lectures_Pdf_Landscape extends VIH_Lectures_Pdf_Base
{
    function __construct()
    {
        parent::__construct('L','mm','A4');
        $this->logo = dirname(__FILE__) . DIRECTORY_SEPARATOR . 'logo.jpg';
    }
    
    function addFrontpage($image)
    {
        $this->addPage();

        $this->Image($this->logo, 100, 55, 100, 0, '', 'http://vih.dk/');

        $this->Image($image, 10, 10, 277,0,'');
        $this->SetFont('Helvetica', 'B', 35);
        $this->setTextColor(255, 255, 255);
        $this->setY(15);
        $this->Cell(0, 35, $this->heading, null, 2, 'C', true);

        $this->addPage();
        $this->SetFont('Helvetica', 'B', 26);
        $this->setTextColor(255, 255, 255);
        $this->Cell(0, 22, $this->heading, null, 2, 'C', true);

        $this->SetFont('Helvetica', null, 18);
        $this->setTextColor(0, 0, 0);
        $this->setY(40);
        $this->writeHtmlCell(275, 10, $this->GetX(), $this->GetY(), $this->description, 0);

        $this->Image($this->logo, 235, 165, 50, 0, '', 'http://vih.dk/');
    }

    function addEventPage($event)
    {
        $title = check_plain($event->title);
        $body_field = field_get_items('node', $event, 'body');
        $body = $body_field[0]['safe_value'];
        $picture = field_get_items('node', $event, 'field_picture');
        $filename = drupal_realpath($picture[0]['uri']);
        $location = trim($event->field_location['und'][0]['safe_value']);
        $price = trim($event->field_price['und'][0]['value']);

        $start_date = new DateTime($event->field_date['und'][0]['value'], new DateTimeZone($event->field_date['und'][0]['timezone']));
        $end_date = new DateTime($event->field_date['und'][0]['value2'], new DateTimeZone($event->field_date['und'][0]['timezone']));
        $date = ucfirst($this->t($start_date->format('l')) . ', ' . $start_date->format('j.') . $this->t($start_date->format('F')) . ', ' . $start_date->format('Y H:i') . '-' . $end_date->format('H:i'));
        $date = format_date(strtotime($event->field_date['und'][0]['value']) + $start_date->getOffset(), 'custom', 'l, j. F, Y H:i', date_default_timezone_get()) . '-' . format_date(strtotime($event->field_date['und'][0]['value2']) + $end_date->getOffset(), 'custom', 'H:i', date_default_timezone_get());

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

        $chart = array(
            '#chart_id' => 'test_chart',
            '#type' => CHART_TYPE_QR,
            '#size' => chart_size(200, 200),
            '#data' => 'http://vih.dk'
        );
        $qr_file = chart_copy($chart, 'my_chart_' . uniqid(), 'public://charts/');
        
        if ($qr_file !== false) {
            $this->Image($qr_file, 235, 135, 45, 0, '');
        }

        $this->Image($this->logo, 235, 180, 50, 0, '', 'http://vih.dk/');

        $this->setY(168);
        $this->SetFont('Helvetica', null, 14);

        $this->MultiCell(200, 12, 'Pris og sted: ' . $price . ' kroner, ' . $location, 0, 'L');

        $this->setY(174);

        $this->Write(14, 'Tid: ' . $date);
        $this->Ln();
    }

    function render($events)
    {
        $this->SetTitle($this->heading);
        $this->SetSubject($this->sub_title);
        $this->SetAuthor('Vejle Idrætshøjskole');
        $this->SetAutoPageBreak(false);

        foreach ($events as $event) {
            $this->addEventPage($event);
        }

        $this->Output('foredrag.pdf', 'I');
    }
}
