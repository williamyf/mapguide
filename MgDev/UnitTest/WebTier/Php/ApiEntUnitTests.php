<?php

//
//  Copyright (C) 2004-2007 by Autodesk, Inc.
//
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of version 2.1 of the GNU Lesser
//  General Public License as published by the Free Software Foundation.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//

require_once("HtmlPrinter.php");
//This script generates the HTML form for Api tests

if (!defined('ApiUnitTests_MAIN_METHOD'))
{
    define('ApiUnitTests_MAIN_METHOD', 'ApiUnitTests::main');
    $test = new ApiUnitTests();
    $test->main();
}

class ApiUnitTests
{
    public function main()
    {
        HtmlPrinter::PrintHtmlHeader("ApiEnt Tests");
        HtmlPrinter::PrintFormHeader("Run.php");

        //Add your tests here
        //HtmlPrinter::PrintTestCases("YourTestName", "Path to database file where your data is stored", "Api")
        HtmlPrinter::PrintTestCases("FeatureService", "../../TestData/FeatureService/FeatureServiceTest.dump", "ApiEnt");

        HtmlPrinter::PrintSelectTestMode();
        HtmlPrinter::PrintSelectOutput();
        HtmlPrinter::PrintFormFooter("ApiEnt", "RunTests");
        HtmlPrinter::PrintHtmlFooter();
    }
}
?>
