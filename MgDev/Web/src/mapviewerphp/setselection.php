<?php

//
//  Copyright (C) 2004-2011 by Autodesk, Inc.
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

    include 'common.php';
    include 'constants.php';

    $mapName = "";
    $sessionId = "";
    $selText = "";
    $queryInfo = false;

    GetRequestParameters();

    try
    {
        InitializeWebTier();

        $cred = new MgUserInformation($sessionId);
        $cred->SetClientIp(GetClientIp());
        $cred->SetClientAgent(GetClientAgent());

        //connect to the site and get an instance of the resoucre service
        //
        $site = new MgSiteConnection();
        $site->Open($cred);
        $resourceSrvc = $site->CreateService(MgServiceType::ResourceService);

        //load the map runtime state
        //
        $map = new MgMap();
        $map->Open($resourceSrvc, $mapName);

        // Create the selection set and save it
        $sel = new MgSelection($map);
        if($selText != "")
            $sel->FromXml($selText);
        $sel->Save($resourceSrvc, $mapName);

        if($queryInfo) {
            //Query feature info for the feature in the selection set. This will return the current set
            //along with property info
            //There must be only one feature in the feature set
            $layers = $sel->GetLayers();
            if($layers == null || $layers->GetCount() != 1) {
                echo "Error: There must be exactly one feature in the set."; ///NOXLATE dbg report only
                return;
            }
            $layer = $layers->GetItem(0);
            $featureClassName = $layer->GetFeatureClassName();
            $filter = $sel->GenerateFilter($layer, $featureClassName);
            $featureSrvc = $site->CreateService(MgServiceType::FeatureService);
            $query = new MgFeatureQueryOptions();
            $query->SetFilter($filter);
            $featureSource = new MgResourceIdentifier($layer->GetFeatureSourceId());
            $features = $featureSrvc->SelectFeatures($featureSource, $featureClassName, $query);
            $featCount = 0;
            while($features->ReadNext())
            {
                if($featCount++ == 1)
                    break;
            }
            $features->Close();
            if($featCount != 1) {
                echo "Error: There must be exactly one feature in the set."; ///NOXLATE dbg report only
                return;
            }
            $renderingSrvc = $site->CreateService(MgServiceType::RenderingService);
            $layerNames = new MgStringCollection();
            $layerNames->Add($layer->GetName());
            $featInfo = $renderingSrvc->QueryFeatures($map, $layerNames, NULL, MgFeatureSpatialOperations::Intersects, $selText, 1, 2);
            header('Content-Type: text/xml; charset: UTF-8');
            echo $featInfo->ToXml()->ToString();
        }
    }
    catch(MgException $e)
    {
        echo "ClearSelection Exception: " . $e->GetDetails();
    }

function GetParameters($params)
{
    global $mapName, $sessionId, $selText, $queryInfo;

    $sessionId = ValidateSessionId(GetParameter($params, 'SESSION'));
    $mapName = ValidateMapName(GetParameter($params, 'MAPNAME'));
    if(isset($params['QUERYINFO']))
        $queryInfo = (GetIntParameter($params, 'QUERYINFO') == 1);

    $selText = UnescapeMagicQuotes(GetParameter($params, 'SELECTION'));
}

function UnescapeMagicQuotes($str)
{
    if(ini_get("magic_quotes_sybase") == "1")
        return str_replace("''", "'", $str);
    else if(get_magic_quotes_gpc() == "1")
    {
        //Unescape double quotes
        $str = str_replace('\\"', '"', $str);

        //remove additional backslash
        return str_replace("\\", "", $str);
    }
    return $str;
}

function GetRequestParameters()
{
    if($_SERVER['REQUEST_METHOD'] == "POST")
        GetParameters($_POST);
    else
        GetParameters($_GET);
}
?>