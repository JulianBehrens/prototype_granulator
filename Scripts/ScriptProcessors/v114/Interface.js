Content.makeFrontInterface(1040, 660);

Console.clear();
Synth.deferCallbacks(true);
Content.setUseHighResolutionForPanels(true);

Engine.loadNextUserPreset(true);




include("BrowserLAF.js");
include("Icons.js");
include("Licence.js");;


//custom fonts

Engine.loadFontAs("{PROJECT_FOLDER}fonts/SpaceGrotesk-Medium.ttf", "font_medium");
Engine.loadFontAs("{PROJECT_FOLDER}fonts/Panchang-Bold.ttf", "font_bold");

Engine.setGlobalFont("font_medium");




//tabbed view

const var NUM_BUTTONS = 2;
const var buttons = [];
const var panels = [];

for (i = 0; i < NUM_BUTTONS; i++)
{
    buttons[i] = Content.getComponent("Button" + (i));
    panels[i] = Content.getComponent("Panel" + (i));
    buttons[i].setControlCallback(onButtonControl);
}

inline function onButtonControl(component, value)
{
	local idx = buttons.indexOf(component);
		
	for (i = 0; i < panels.length; i++)
    {
        panels[i].showControl(idx == i);
        buttons[i].setValue(i == idx && value);
    }
	
}

onButtonControl(buttons[0], true);



// HOW TO USE
// - Name your parameter
// - set your parameter type
// - make sure to set the correct middlePosition
// - adjust the mouse sensitivity
// add to Knobs array or use Knob.setLocalLookAndFeel(LAF_Knob);

// How to add custom Icons
// At the end of the file you can find the icons. I recommend to put this into a namespace called Icons. Using the hise svg importer add the path as a HISE number path. Do not use SVG only number path as this is stylable

// Use this format to assign a icon to a slider:
// ParameterName$iconName
// its important to add the $ 
// everything after the $ is interepreted as icon name
// make sure you icons all have the same spacings.

// You can find a config to tweak the look and feel and adjust it to your spacing. 

const var LAF_Knob = Content.createLocalLookAndFeel();

// add the knobs here that you want to style
const var Knobs = [Content.getComponent("Knob1"),
                   Content.getComponent("Knob2"),
                   Content.getComponent("Knob3"),
                   Content.getComponent("Knob4a"),
                   Content.getComponent("Knob4b"),
                   Content.getComponent("Knob5"),
                   Content.getComponent("Knob6"),
                   Content.getComponent("Knob7a"),
                   Content.getComponent("Knob7b"),
                   Content.getComponent("Knob8a"),
                   Content.getComponent("Knob8b")];


for (knb in Knobs) {
	
	// assign knob to local look and feel
	knb.setLocalLookAndFeel(LAF_Knob);
	
}

// LAF FUNCTION
LAF_Knob.registerFunction('drawRotarySlider', knobLAF);



inline function knobLAF(g, obj) {
	
	local a = obj.area;
	
	// TWEAK THIS SETTINGS TO FIT YOUR SCALING 
	local ICON_SIZE = 20;
	local BORDER_RADIUS = 1;
	
	// local SLIDER_BORDER_RADIUS = 1; ROUNDED SLIDER :)
	local SLIDER_BORDER_RADIUS = 0;
	local SLIDER_THICKNESS = 3;
	
	// SPACING
	local LEFT_PADDING = 15;
	local RIGHT_PADDING = 15;
	
	local ICON_SLIDER_GAP = 10;
	local LABEL_SLIDER_GAP = 6;
	local LABEL_SLIDER_TOP_PADDING = 3;
	
	// FONTS
	local LABEL_FONT = "font_medium";
	local LABEL_FONT_SIZE = 20;
	local LABEL_FONT_SPACING = 0.04;
	
	local VALUE_FONT = "font_bold";
	local VALUE_FONT_SIZE = 20;
	local VALUE_FONT_SPACING = 0.04;
	
	// COLORS
	local SLIDER_BACKGROUND_COLOR = Colours.withAlpha(obj.bgColour,0.2);
	
	// USE OBJ COLORS;
	// I recommend to use custom colors here
	local bgColor = obj.bgColour;
	local invertedbgColor = obj.itemColour1;
	local textColor = obj.textColour;
	local invertedTextCol = obj.itemColour2;
	
	local value = obj.valueAsText;
	local normalizedValue = obj.valueNormalized;
	local text = obj.text.split('$');
	local label = text[0];
	local icon = text[1];
	local isBipolar = obj.text.contains('$isBipolar');
	
	local iconArea = [
		a[0] + LEFT_PADDING,
		a[1] + a[3] / 2 - ICON_SIZE / 2,
		ICON_SIZE,
		ICON_SIZE
	];
	
	local rightArea = [
		a[0] + ICON_SIZE * 2 + ICON_SLIDER_GAP,
		a[1] + LABEL_SLIDER_TOP_PADDING,
		a[2] - ICON_SIZE * 2 - ICON_SLIDER_GAP - RIGHT_PADDING,
		a[3]
	];
	
	local labelArea = [
		rightArea[0],
		rightArea[1] - LABEL_SLIDER_GAP,
		rightArea[2],
		rightArea[3]
	];
	
	local sliderArea = [
		rightArea[0],
		rightArea[1] + rightArea[3] / 2 + LABEL_SLIDER_GAP,
		rightArea[2],
		SLIDER_THICKNESS
	];
	
	local sliderWithValue = [
		rightArea[0],
		rightArea[1] + rightArea[3] / 2 + LABEL_SLIDER_GAP,
		rightArea[2] * normalizedValue,
		SLIDER_THICKNESS
	];
	
	if (isBipolar) {
		local newValue = -1 * (0.5 - normalizedValue);
	    sliderWithValue = [
			rightArea[0] + rightArea[2] / 2,
			rightArea[1] + rightArea[3] / 2 + LABEL_SLIDER_GAP,
			rightArea[2] * newValue,
			SLIDER_THICKNESS
	    ];
	}
	
	if (obj.hover || obj.clicked) {
		bgColor = invertedbgColor;
		textColor = invertedTextCol;
	}

	g.setColour(bgColor);
	g.fillRoundedRectangle(a, BORDER_RADIUS);
	
	g.setColour(SLIDER_BACKGROUND_COLOR);
	g.fillRoundedRectangle(sliderArea, SLIDER_BORDER_RADIUS);
	
	g.setColour(textColor);
	g.fillPath(icons[icon], iconArea);
	
	g.setFontWithSpacing(LABEL_FONT, LABEL_FONT_SIZE, LABEL_FONT_SPACING);
	g.drawAlignedText(label, labelArea, 'left');
	
	g.setFontWithSpacing(VALUE_FONT, VALUE_FONT_SIZE, VALUE_FONT_SPACING);
	g.drawAlignedText(value, labelArea, 'right');
	g.fillRoundedRectangle(sliderWithValue, SLIDER_BORDER_RADIUS);
}


//"drawSliderPackBackground",
//"drawSliderPackFlashOverlay",
//"drawSliderPackRightClickLine",
//"drawSliderPackTextPopup"


const var SliderPack1 = Content.getComponent("SliderPack1");

const var SPLAF = Content.createLocalLookAndFeel();

SPLAF.registerFunction("drawLinearSlider", function(g, obj)
{
	// ID is the number of the slider
	var a = obj.area;
	var v = obj.valueNormalized;
	var l = Colours.pink;
	var r = Colours.blue;
		

	var c = Colours.mix(l, r, parseInt(obj.id) / 16);


	
	g.setColour(0xFFDDE8F3);

	{
		a[1] = a[1]+8 + a[3]/ 1.1 * (1.0 - v);
		a[3] = a[3] - a[1];
	}		
	

//stripes
//mid
g.setGradientFill([(0xFFDDE8F3), 2, 0.1* obj.area[3], (0x108AD5FF), 1.0*obj.area[2] / 1000 - 2, 1.4*obj.area[3]]);
g.fillRoundedRectangle([a[0]+26, a[1]+1, a[2]-a[2]+2, a[3]-1], 1.0);

//left
g.setColour(0x20DDE8F3);
g.fillRoundedRectangle([a[0]+30, a[1]+1, a[2]-a[2]+2, a[3]-1], 1.0);

//right
g.setColour(0x40DDE8F3);
g.fillRoundedRectangle([a[0]+22, a[1]+1, a[2]-a[2]+2, a[3]-1], 1.0);




//mid
g.setColour(0xFFDDE8F3);
g.fillRoundedRectangle([a[0]-a[0]+13, a[1]-6, a[2]-a[2]+28, a[3]-a[3]+5], 1.0);

//left
g.setColour(0x60DDE8F3);
g.fillRoundedRectangle([a[0]+6, a[1]-6, a[2]-a[2]+5, a[3]-a[3]+5], 1.0);

//right
g.setColour(0x60DDE8F3);
g.fillRoundedRectangle([a[0]+43, a[1]-6, a[2]-a[2]+5, a[3]-a[3]+5], 1.0);



});

SliderPack1.setLocalLookAndFeel(SPLAF);


SPLAF.registerFunction("drawSliderPackFlashOverlay", function(g, obj)
{
var a = obj.area;	


g.setColour(0x10DFE8F2);
g.fillRoundedRectangle(obj.area, 3.0);
{
	a[1] = a[1] + a[3]-2 * (1.0);
	a[3] = a[3] - a[1];

}	




g.setColour(0x10DFE8F2);
g.fillRoundedRectangle(obj.area, 3.0);

g.setColour(0xFFDFE8F2);
g.fillRoundedRectangle([a[0]-26, a[1]+1, a[2]+2, a[3]+10], 1.0);

});



//toggle knobs;

const var Buttonsync = Content.getComponent("Buttonsync");
const var Buttonsync1 = Content.getComponent("Buttonsync1");
const var A = Content.getComponent("Knob4a");
const var B = Content.getComponent("Knob4b");
const var A1 = Content.getComponent("Knob7a");
const var A2 = Content.getComponent("Knob7b");
const var B1 = Content.getComponent("Knob8a");
const var B2 = Content.getComponent("Knob8b");
const var GRAINS = Synth.getEffect("grains")


inline function onButtonsyncControl(number, value)
{
        GRAINS.setAttribute(GRAINS.TempoSync, value);
		A.showControl(value);
        B.showControl(1-value);    
}

inline function onButtonsync1Control(number, value)
{
        GRAINS.setAttribute(GRAINS.Delay_Sync, value);
		A1.showControl(value);
		B1.showControl(value);
        
        A2.showControl(1-value);  
        B2.showControl(1-value);   
}


Buttonsync.setControlCallback(onButtonsyncControl);
Buttonsync1.setControlCallback(onButtonsync1Control);


//zooom

namespace ZoomHandler
{
	const var MIN_ZOOM = 0.85;
	const var MAX_ZOOM = 2.0;
	const var ZOOM_STEP = 0.25;
	const var INTERFACE_WIDTH = 897;
	const var INTERFACE_HEIGHT = 505;

	const var ZoomPanel = Content.getComponent("ZoomPanel");
	
	const var draggerData = [ 110,109,143,130,218,67,147,216,145,66,108,147,216,145,66,143,130,218,67,108,0,0,0,0,143,130,218,67,108,143,130,218,67,0,0,0,0,108,143,130,218,67,147,216,145,66,99,109,143,130,218,67,139,140,96,67,108,139,140,96,67,143,130,218,67,108,66,160,23,67,143,
	130,218,67,108,143,130,218,67,66,160,23,67,108,143,130,218,67,139,140,96,67,99,109,143,130,218,67,102,22,188,67,108,102,22,188,67,143,130,218,67,108,66,160,151,67,143,130,218,67,108,143,130,218,67,66,160,151,67,108,143,130,218,67,102,22,188,67,99,101,
	0,0 ];
	
	
	const var draggerPath = Content.createPath();
	
	
	draggerPath.loadFromData(draggerData);
	
	ZoomPanel.setPaintRoutine(function(g)
	{
		g.setColour(Colours.withAlpha(Colours.black, (this.data.hover && this.data.allowDrag) ? 0.9 : 0.6));
		g.fillPath(draggerPath, [8, 4, 14, 14]);
	});
	
	inline function allowZoom(panel, on)
	{
		panel.data.allowDrag = on;
		panel.setMouseCursor(on ?"BottomRightCornerResizeCursor" : "NormalCursor", Colours.white, [0, 0]);
		panel.repaint();
	}
	
	allowZoom(ZoomPanel, true);
	
	ZoomPanel.setMouseCallback(function(event)
	{
		this.data.hover = event.hover;
		
		if(event.clicked)
		{
			this.data.zoomStart = Settings.getZoomLevel();
		}
		if(event.mouseUp)
		{
			return;
		}
	
		if(event.drag)
		{
			if(!this.data.allowDrag)
				return;
	
			var diagonal = Math.sqrt(INTERFACE_WIDTH*INTERFACE_WIDTH + INTERFACE_HEIGHT*INTERFACE_HEIGHT);
			var currentZoom = Settings.getZoomLevel();
			var dragPixel = 0;
			
			if(event.dragX > event.dragY)
				dragPixel = (event.dragX * currentZoom) / INTERFACE_WIDTH;
			else
				dragPixel = (event.dragY * currentZoom) / INTERFACE_HEIGHT;
			
			
			var maxScaleFactor = Content.getScreenBounds(false)[3] / INTERFACE_HEIGHT;
			var diagonalDrag = this.data.zoomStart + dragPixel;
			
			diagonalDrag += (ZOOM_STEP / 2);
			
			diagonalDrag = Math.min(diagonalDrag, maxScaleFactor);
			
			diagonalDrag -= Math.fmod(diagonalDrag, ZOOM_STEP);
			diagonalDrag = Math.range(diagonalDrag, MIN_ZOOM, MAX_ZOOM);
			
			var zoomToUse = diagonalDrag;
	
			if(currentZoom != zoomToUse)
			{
				Settings.setZoomLevel(zoomToUse);
			}
		}
		
		this.repaint();
	});
}



function onNoteOn()
{
	
}
 function onNoteOff()
{
	
}
 function onController()
{
	
}
 function onTimer()
{
	
}
 function onControl(number, value)
{
	
}
 