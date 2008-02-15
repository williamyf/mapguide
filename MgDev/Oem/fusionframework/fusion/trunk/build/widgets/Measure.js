/**
 * Fusion.Widget.Measure
 *
 * $Id: Measure.js 1228 2008-02-14 21:04:43Z madair $
 *
 * Copyright (c) 2007, DM Solutions Group Inc.
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

 /* ********************************************************************
 * Class: Fusion.Widget.Measure
 *
 * The Measure widget allows the user to measure distances or areas on the map 
 * in one or more segments. Area is positive if measured clockwise.
 * 
 * **********************************************************************/

Fusion.Constant.MEASURE_TYPE_DISTANCE = 0;
Fusion.Constant.MEASURE_TYPE_AREA = 1;
Fusion.Constant.MEASURE_TYPE_BOTH = 2;

Fusion.Event.MEASURE_NEW_SEGMENT = Fusion.Event.lastEventId++;
Fusion.Event.MEASURE_SEGMENT_UPDATE = Fusion.Event.lastEventId++;
Fusion.Event.MEASURE_SEGMENT_COMPLETE = Fusion.Event.lastEventId++;
Fusion.Event.MEASURE_CLEAR = Fusion.Event.lastEventId++;

Fusion.Widget.Measure = Class.create();
Fusion.Widget.Measure.prototype = {
    isDigitizing: false,
    //distance of each segment
    distances: null,
    distanceMarkers: null,
    
    //Array of points used to compute area
    aAreaFirstPoint: null,
    //cumulativeDistance
    cumulativeDistance: 0,
    lastDistance: 0,
    //for areas
    //cumulativeArea
    cumulativeArea: 0,
    lastArea: 0,
    aAreas: [],
    
    /* the units to display distances in */
    units: Fusion.UNKNOWN,

    /* Type of measure: values = disance, area or both, default: both*/
    mType: null,

    /* Precision of the distance displayed */
    distPrecision: 0,
    
    /* Precision of the area displayed */
    areaPrecision: 0,
    
    /* an HTML container to put the current distance in */
    measureTip: null,
        
    /* Static position of Tooltip Box TOP */
    measureTipPositionTop: null,
   
    /* Static position of Tooltip Box LEFT */ 
    measureTipPositionLeft: null,
    
    /* Tooltip appearance: static or dynamic */
    tooltipType: '',

    /* Style for the distance line used for distance draw */   
    distanceNormalStyle: null,

    /* Style for the polygon used for area draw */   
    fillStyle: null,

    /* Style for the polygon line used for area draw */    
    areaStyle: null,
    
    initialize : function(widgetTag) {
        Object.inheritFrom(this, Fusion.Widget.prototype, [widgetTag, true]);
        Object.inheritFrom(this, Fusion.Tool.ButtonBase.prototype, []);
        Object.inheritFrom(this, Fusion.Tool.Canvas.prototype, []);
        this.asCursor = ['crosshair'];
        var json = widgetTag.extension;
        this.units = (json.Units && (json.Units[0] != '')) ?  Fusion.unitFromName(json.Units[0]): this.units;
        this.distPrecision = json.DistancePrecision ? json.DistancePrecision[0] : 2;
        this.areaPrecision = json.AreaPrecision ? json.AreaPrecision[0] : 2;  
        
        this.sTarget = json.Target ? json.Target[0] : "";
        this.sBaseUrl = Fusion.getFusionURL() + 'widgets/Measure/Measure.php';
        
              
        //init measure type
        this.measureType = Fusion.Constant.MEASURE_TYPE_BOTH;
        if (json.Type) {
            switch(json.Type[0].toLowerCase()) {
                case 'distance':
                    this.measureType = Fusion.Constant.MEASURE_TYPE_DISTANCE;
                    break;
                case 'area':
                    this.measureType = Fusion.Constant.MEASURE_TYPE_AREA;
                    break;
            }
        }
        //we don't support area yet.
        this.measureType = Fusion.Constant.MEASURE_TYPE_DISTANCE;
        
        //Here are the canvas style definition
        var fillStyle = json.FillStyle ? json.FillStyle[0] : 'rgba(0,0,255, 0.3)';
        var lineStyleWidth = json.LineStyleWidth ? json.LineStyleWidth[0] : 2;
        var lineStyleColor = json.LineStyleColor ? json.LineStyleColor[0] : 'rgba(0,0,255,0.3)';     
        this.fillStyle = new Fusion.Tool.Canvas.Style({fillStyle:fillStyle});
        this.lineStyle = new Fusion.Tool.Canvas.Style({lineWidth:lineStyleWidth,strokeStyle:lineStyleColor});  	
        this.distanceMarkers = [];
        this.distances = [];
        
        this.registerEventID(Fusion.Event.MEASURE_NEW_SEGMENT);
        this.registerEventID(Fusion.Event.MEASURE_SEGMENT_COMPLETE);
        this.registerEventID(Fusion.Event.MEASURE_SEGMENT_UPDATE);
        this.registerEventID(Fusion.Event.MEASURE_CLEAR);
        this.getMap().registerForEvent(Fusion.Event.MAP_EXTENTS_CHANGED, this.resetCanvas.bind(this));
        this.keyHandler = this.onKeyPress.bind(this);
        Fusion.addWidgetStyleSheet(widgetTag.location + 'Measure/Measure.css');

        this.getMap().registerForEvent(Fusion.Event.MAP_LOADED, this.setUnits.bind(this));
        this.registerParameter('Units');
        
    },
    
    onKeyPress: function(e) {
        //console.log('Rule::onKeyPress');
        var charCode = (e.charCode ) ? e.charCode : ((e.keyCode) ? e.keyCode : e.which);
        //console.log(charCode);
        if (charCode == Event.KEY_ESC) {
            this.resetCanvas();
        } 
    },
    
    /**
     * (public) activate()
     *
     * activate the measure tool
     */
    activateTool: function() {
        this.getMap().activateWidget(this);
        this._oButton.activateTool();
    },

    /**
     * (public) initVars()
     *
     * reset area and/or distance vars
     */    
    initVars: function() {
        this.aDistances = [];
        this.aAreas = [];
        this.cumulativeDistance = 0;
        this.lastDistance = 0;
        this.cumulativeArea = 0;
        this.lastArea = 0;
        this.aAreaFirstPoint = null;
    },
    
    activate: function() {
        this.activateCanvas();
        this.initVars();
        this.triggerEvent(Fusion.Event.MEASURE_CLEAR, this);
        Event.observe(document,"keypress",this.keyHandler);
        this.showPanel();
    },
    
    showPanel : function() {
        if (this.sTarget) {
            var url = this.sBaseUrl;
            var queryStr = 'locale='+Fusion.locale;
            if (url.indexOf('?') < 0) {
                url += '?';
            } else if (url.slice(-1) != '&') {
                url += '&';
            }
            url += queryStr;
            
            var taskPaneTarget = Fusion.getWidgetById(this.sTarget);
            var pageElement = $(this.sTarget);
            if ( taskPaneTarget ) {
                taskPaneTarget.setContent(url);
            } else {
                if ( pageElement ) {
                    pageElement.src = url;
                } else {
                    window.open(url, this.sTarget, this.sWinFeatures);
                }
            }
        }
    },    
    
    /**
     * (public) deactivate()
     *
     * deactivate the ruler tool
     */
    deactivate: function() {
        //console.log('Ruler.deactivate');
        Event.stopObserving(document, 'keypress', this.keyHandler);           
        this._oButton.deactivateTool();
        this.deactivateCanvas();
        this.resetCanvas();
    },
    
    resetCanvas: function() {
        if (this.isDigitizing) {
            this.isDigitizing = false;
        }
        this.clearContext();
        this.initVars();
        for (var i=0; i<this.distanceMarkers.length; i++)  {
            this.distanceMarkers[i].destroy();
        }
        this.distanceMarkers = [];
        this.triggerEvent(Fusion.Event.MEASURE_CLEAR, this);
    },
      
    /**
     * (public) mouseDown(e)
     *
     * handle the mouse down event
     *
     * @param e Event the event that happened on the mapObj
     */
    mouseDown: function(e) {  	
        if (Event.isLeftClick(e)) {
            var map = this.getMap();
            var p = map.getEventPosition(e);
            var gp = map.pixToGeo(p.x, p.y);
            
            if (!this.isDigitizing) {
                this.resetCanvas();
                var from = new Fusion.Tool.Canvas.Node(gp.x,gp.y, map);
                var to = new Fusion.Tool.Canvas.Node(gp.x,gp.y, map);
                var lastSegment = new Fusion.Tool.Canvas.Segment(from,to);
                if (this.measureType == Fusion.Constant.MEASURE_TYPE_DISTANCE) {
                    this.feature = new Fusion.Tool.Canvas.Line(map);
                    this.feature.lineStyle = this.lineStyle;
                } else {
                    this.feature = new Fusion.Tool.Canvas.Polygon(map);
                    this.feature.fillStyle = this.fillStyle;
                    this.feature.lineStyle = this.lineStyle;
                }
                this.feature.addSegment(lastSegment);
                this.aAreaFirstPoint = new Fusion.Tool.Canvas.Node(gp.x,gp.y, map);
                this.isDigitizing = true;                                  
            } else {
                //if digitizing
                var lastSegment = this.feature.lastSegment();
                lastSegment.to.set(gp.x,gp.y);
                if (lastSegment.from.x == lastSegment.to.x && 
                    lastSegment.from.y == lastSegment.to.y) {
                    this.dblClick(e);
                    return;
                }
                this.feature.extendLine();
                this.updateMarker(this.lastMarker, lastSegment, e);
            }
            //create a new marker
            this.lastMarker = new Fusion.Widget.Measure.DistanceMarker(this.units);
            this.distanceMarkers.push(this.lastMarker);
            this.clearContext();
            this.feature.draw(this.context);
        }
    },

    /**
     * (public) mouseMove(e)
     *
     * handle the mouse move event
     *
     * @param e Event the event that happened on the mapObj
     */
    mouseMove: function(e) {
        if (!this.isDigitizing) {
            return;
        }
        var offset = {x:0,y:0};
        var oElement = this.getMap().getDomObj();
        var target = e.target || e.srcElement;
        if (target.id != 'featureDigitizer') { 
            return;
        }
        if (this.delayUpdateTimer) {
            window.clearTimeout(this.delayUpdateTimer);
        }
        var map = this.getMap();
        var p = map.getEventPosition(e);
        var gp = map.pixToGeo(p.x, p.y);
        
        var lastSegment = this.feature.lastSegment();
        lastSegment.to.set(gp.x,gp.y);
        this.clearContext();
        this.feature.draw(this.context);
        this.delayUpdateTimer = window.setTimeout(this.delayUpdate.bind(this, lastSegment, e), 250);
        this.lastMarker.setCalculating();
        this.triggerEvent(Fusion.Event.MEASURE_SEGMENT_UPDATE, this, this.lastMarker, null);
        
        this.positionMarker(this.lastMarker, lastSegment);
    },
    
    delayUpdate: function(lastSegment, e) {
        this.delayUpdateTimer = null;
        this.updateMarker(this.lastMarker, lastSegment, e);
    },
   
    /**
     * (public) dblClick(e)
     *
     * handle the mouse dblclick event
     *
     * @param e Event the event that happened on the mapObj
     */
    dblClick: function(e) {
        //console.log('Digitizer.dblClick');
        if (!this.isDigitizing) {
            return;
        }
        var p = this.getMap().getEventPosition(e);
        var gp = this.getMap().pixToGeo(p.x, p.y);   
        var seg = this.feature.lastSegment();
        seg.to.set(gp.x,gp.y);
        this.clearContext();
        this.feature.draw(this.context);
        
        if (this.measureType == Fusion.Constant.MEASURE_TYPE_AREA || this.measureType == Fusion.Constant.MEASURE_TYPE_BOTH) {
            
        }
        if (this.measureType == Fusion.Constant.MEASURE_TYPE_DISTANCE || this.measureType == Fusion.Constant.MEASURE_TYPE_BOTH) {
        }  

        this.isDigitizing = false;
    },
    
    positionMarker: function(marker, segment) {
        var oDomElem =  this.getMap().getDomObj();
        if (!marker.domObj.parentNode || 
            marker.domObj.parentNode != oDomElem) {
            oDomElem.appendChild(marker.domObj);
            this.triggerEvent(Fusion.Event.MEASURE_NEW_SEGMENT, this, marker);
        }
        var size = marker.getSize();
        var t = (segment.from.py + segment.to.py - size.height)/2 ;
        var l = (segment.from.px + segment.to.px - size.width)/2;
        marker.domObj.style.top = t + 'px';
        marker.domObj.style.left = l + 'px';
    },
    
    updateMarker: function(marker, segment, e) {
        if (!marker) {
            return;
        }
        this.measureSegment(segment, marker);
        this.positionMarker(marker, segment);
    },
    
    measureSegment: function(segment, marker) {
        var points = '&x1='+segment.from.x+'&y1='+segment.from.y+
                     '&x2='+segment.to.x+'&y2='+segment.to.y;
        var map = this.getMap();
        var aMaps = map.getAllMaps();
        var s = aMaps[0].arch + '/' + Fusion.getScriptLanguage() + "/Measure." + Fusion.getScriptLanguage() ;
        var sessionId = aMaps[0].getSessionID();
        var params = {};
        params.parameters = 'session='+sessionId+'&locale='+Fusion.locale+'&mapname='+ this.getMap().getMapName()+points;
        params.onComplete = this.measureCompleted.bind(this, segment, marker);
        Fusion.ajaxRequest(s, params);
    },
    
    measureCompleted: function(segment, marker, r, json) {
        if (json && r.status == 200) {
            var o;
            eval('o='+r.responseText);
            if (o.distance) {
              /* distance returned is always in meters*/
              //var mapUnits = Fusion.unitFromName(this.getMap().getUnits());
              //if (mapUnits == Fusion.DEGREES || Fusion.DECIMALDEGREES)
              mapUnits = Fusion.METERS;

              if (mapUnits != this.units) {
                o.distance = Fusion.convert(mapUnits, this.units, o.distance);
              }
              var p = Math.pow(10,this.distPrecision);
              var d = Math.round(o.distance*p)/p;
              
              marker.setDistance(d);
              this.positionMarker(marker, segment);
                if (segment == this.feature.lastSegment()) {
                    this.triggerEvent(Fusion.Event.MEASURE_SEGMENT_UPDATE, this, marker, d);                    
                } else {
                    this.triggerEvent(Fusion.Event.MEASURE_SEGMENT_COMPLETE, this, marker, d);
                }
            }
        }
    },
    
    setUnits: function() {
      if (this.units == Fusion.UNKNOWN) {
        this.setParameter('Units',this.getMap().getUnits());
      }
    },

    setParameter: function(param, value) {
      //console.log('setParameter: ' + param + ' = ' + value);
        if (param == 'Units') {
            this.units = Fusion.unitFromName(value);
            for (var i=0; i<this.distanceMarkers.length; i++) {
                this.distanceMarkers[i].setUnits(this.units);
            }
        }
    }
};

Fusion.Event.MARKER_DISTANCE_CHANGED = Fusion.Event.lastEventId++;

Fusion.Widget.Measure.DistanceMarker = Class.create();
Fusion.Widget.Measure.DistanceMarker.prototype = {
    calculatingImg: null,
    isCalculating: false,
    distance: null,
    initialize: function( units) {
        Object.inheritFrom(this, Fusion.Lib.EventMgr, []);
        this.registerEventID(Fusion.Event.MARKER_DISTANCE_CHANGED);
        this.domObj = document.createElement('div');
        this.domObj.className = 'divMeasureMarker';
        this.setUnits(units);
        this.calculatingImg = document.createElement('img');
        this.calculatingImg.src = Fusion.getFusionURL() + 'widgets/Measure/MeasurePending.gif';
        this.calculatingImg.width = 19;
        this.calculatingImg.height = 4;
        this.setCalculating();
    },
    
    destroy: function() {
        Fusion.Lib.EventMgr.destroy.apply(this, []);
        if (this.domObj.parentNode) {
            this.domObj.parentNode.removeChild(this.domObj);
        }
    },
    
    setUnits: function(units) {
        this.unit = units;
        this.unitAbbr = Fusion.unitAbbr(units);
        if (this.distance) {
            this.setDistance(this.distance);
        }
    },
    
    getDistance: function() {
        return this.distance;
    },
    
    getDistanceLabel: function() {
        if (this.distance) {
            var distance = this.distance;
            return distance + ' ' + this.unitAbbr;            
        } else {
            return false;
        }
    },
    
    setDistance: function(distance) {
        if (this.calculatingImg.parentNode) {
            this.calculatingImg.parentNode.removeChild(this.calculatingImg);
        }
        this.distance = distance;
        this.domObj.innerHTML = this.getDistanceLabel();
        this.isCalculating = false;
        this.triggerEvent(Fusion.Event.MARKER_DISTANCE_CHANGED, this);
    },
    
    setCalculating: function() {
        if (!this.isCalculating) {
            this.isCalculating = true;
            this.domObj.innerHTML = '';
            this.domObj.appendChild(this.calculatingImg);
            this.distance = false;
            this.triggerEvent(Fusion.Event.MARKER_DISTANCE_CHANGED, this);
        }
    },
    getSize: function() {
        var size =  Element.getDimensions(this.domObj);
        var imgSize = {width:19, height:4};
        if (size.width < imgSize.width) {
            size.width += imgSize.width;
        }
        if (size.height < imgSize.height) {
            size.height += imgSize.height;
        }
        
        return size;
    }
};
