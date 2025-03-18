//Licence

namespace Activation
{
	// Set the base URL we'll use later on when we'll send the activation data
	//Server.setBaseURL("https://api.lemonsqueezy.com");
	// Set the HTTP header according to Lemon Squeezy's API docs
	//Server.setHttpHeader("Accept: application/json");
	
	// We'll store the user license details in a file in the AppData folder, so we'll be able to periodically check if the plugin has been correctly activated
	const appData = FileSystem.getFolder(FileSystem.AppData);
	const register = appData.getChildFile("Bin.dat");
	
	// We'll store the user data in this variable object
	reg userDetails = 
	{
		// This key will be read by the scipt on plugin launch
		Activated: false,
		Email : "",
		License : "",
		// The instance is used to periodically check if the license is valid
		InstanceID : "",
		// This can be used if you want to display the user's name in the GUI
		Username : "",
	};
	
	// This key will be used to encrypt the user's data, and store it on his computer
	const encryptionKey = "5XZsAffP355GCYE07tzOmHve9enmaY8g";
	
	// Set the length of the distributed license keys - it will be used to check if the format of a submitted license is valid and assumes this never changes
	const LICENSE_LENGTH = 36;
	// The ID of your product
	const PRODUCT_ID = 197922;
	
	// ASsuming you're using a MIDI muter
	const MidiMuter1 = Synth.getMidiProcessor("MidiMuter1");
	
	// Declare an array of all possible error messages
	const NOTICE = 
	{
		INVALID_EMAIL : "Invalid Email.",
		INVALID_LICENSE : "Invalid License.",
		LICENSE_NOT_FOUND : "License Key not found.",
		SERVER_ERROR : "Server Error. Please try again or contact support.",
		NO_CONNEXION : "Connexction Error. Please check your network settings.",
		EMAIL_MISMATCH : "You don't own this license.",
	}
	
	// ------------------- GUI SETUP ------------------------------------------------------------------------------
	
	// Create a panel that will wrap all the activation widgets
	const pnlActivationWrapper = Content.addPanel("pnlActivationWrapper", 0, 0);
	const activationWrapperProperties = 
	{
		//width: 400,
		//height: 300,
		//itemColour: 0xFF161616,
		//itemColour2: 0xFF202020,
	}
	// ... and set its properties
	Content.setPropertiesFromJSON("pnlActivationWrapper", activationWrapperProperties);
	
	// Create the Activation button
	const btnAuthorize = Content.addButton("btnAuthorize", 430, 410);
	const btnActivateProperties = 
	{
		width: 178,
		height: 35,
		isMomentary: true,
		saveInPreset: false,
		parentComponent: "pnlActivationWrapper"
	}
	// ... and set its properties
	Content.setPropertiesFromJSON("btnAuthorize", btnActivateProperties);
	
	// Create the label to input the user's email
	const lblInputEmail = Content.addLabel("lblInputEmail", 380, 260);
	const inputEmailProperties = 
	{
		width: 280,
		height: 17,
		enabled: true,
		saveInPreset: false,
		text: "e-mail address",
		alignment: "left",
		parentComponent: "pnlActivationWrapper"
	}
	// ... and set its properties
	Content.setPropertiesFromJSON("lblInputEmail", inputEmailProperties);
	
	// Create the label to input the user's license
	const lblInputLicense = Content.addLabel("lblInputLicense", 381, 312);
	const inputLicenseProperties = 
	{
		width: 280,
		height: 17,
		enabled: true,
		saveInPreset: false,
		text: "product licence",
		alignment: "left",
		parentComponent: "pnlActivationWrapper"
	}
	// ... and set its properties
	Content.setPropertiesFromJSON("lblInputLicense", inputLicenseProperties);
	
	// Create a label that will display messages to the user
	const var lblNotice = Content.addLabel("lblNotice", 351, 367);
	const noticeProperties = 
	{
		width: 340,
		height: 30,
		saveInPreset: false,
		text: "",
		visible: false,
		parentComponent: "pnlActivationWrapper"
	}
	// ... and set its properties
	Content.setPropertiesFromJSON("lblNotice", noticeProperties);
	
	// Create a panel that will contain the activation success notice
	const pnlActivationSuccess = Content.addPanel("pnlActivationSuccess", 350, 100);
	const successWrapperProperties = 
	{
		width: 340,
		height: 440,
		itemColour: 0xFF0F1011,
		itemColour2: 0xFF0F1011,
		visible: false,
		parentComponent: "pnlActivationWrapper",
	}
	// ... and set its properties
	Content.setPropertiesFromJSON("pnlActivationSuccess", successWrapperProperties);
	
	// Create the label to tell the user that the plugin has been activated
	const lblActivationSuccess = Content.addLabel("lblActivationSuccess", 89, 185);
	const lblSuccessProperties = 
	{
		width: 160,
		height: 30,
		text: "Activation successful!",
		parentComponent: "pnlActivationSuccess",
	}
	// ... and set its properties
	Content.setPropertiesFromJSON("lblActivationSuccess", lblSuccessProperties);
	
	// Create the close-and-hide-the-activation-panel button
	const btnCloseAfterActivation = Content.addButton("btnCloseAfterActivation", 105, 248);
	const btnCloseProperties = 
	{
		width: 130,
		height: 30,
		isMomentary: true,
		saveInPreset: false,
		text: "OK",
		parentComponent: "pnlActivationSuccess"
	}
	// ... and set its properties
	Content.setPropertiesFromJSON("btnCloseAfterActivation", btnCloseProperties);
	
	// Create a panel that will be our loading bar (or spinner) and some useful constants
	const START = true;
	const STOP = false;
	const pnlWait = Content.addPanel("pnlWait",502, 131);
	const waitProperties = 
	{
		width: 40,
		height: 40,
		visible: false,
		parentComponent: "pnlActivationWrapper"
	}
	// ... and set its properties
	Content.setPropertiesFromJSON("pnlWait", waitProperties);
	
	// Put the loading spinner start/stop in a separated function 
	inline function startSpinner(startStop)
	{
		if(startStop == true)
			pnlWait.startTimer(50);
		else
			pnlWait.stopTimer();
		
		// This will grey out the labels so they appear "unavailable" during the server call
		lblInputEmail.set("enabled", !startStop);
		lblInputLicense.set("enabled", !startStop);
		
		pnlWait.showControl(startStop);
	}
	
	// ------------------- FUNCTIONS ------------------------------------------------------------------------------
	
	// The function that will be triggered when the user clicks the Activation button
	inline function onbtnAuthorizeControl(component, value)
	{
		if(!value)
			return;
		
		local e = lblInputEmail.get("text");
		local l = lblInputLicense.get("text");
		
		// Check if the user input are valid using cutom function below
		local inputIsValid = UserInputCheck(e, l);
		
		// If the user inputs are valid, then we can submit the user's data to the server and check if the license is valid
		if(inputIsValid)
			SubmitLicense(e, l);
	};
	btnAuthorize.setControlCallback(onbtnAuthorizeControl);
	
	// ------------------- NOT MANDATORY
	// This function will verify that the email and licenses are properly formatted and are valid
	// If the email and/or the license are not properly formatted, an error notice will be displayed, and the autorisation process will stop
	// You can perform a check of these inputs using this function, or let the API reponse answer for you if something is not properly formatted
	inline function UserInputCheck(email, license)
	{
		if(email.indexOf("@") == -1 || email == "Enter your email..." || email == "")
		{
			lblNotice.showControl(true);
			lblNotice.set("text", NOTICE.INVALID_EMAIL);
			return false;
		}
		
		if(license.length != LICENSE_LENGTH || license == "Enter your license..." || license == "")
		{
			lblNotice.showControl(true);
			lblNotice.set("text", NOTICE.INVALID_LICENSE);
			return false;
		}
		
		return true;
	}
	// ----------------------------------------------------------------------------------------------------------------------
	
	inline function SubmitLicense(email, license)
	{
		// Get the user inputs and format the into a JSON object
		local data = 
		{
			license_key : license,
			instance_name : "Test"
		};
	
		// Store the email the user has entered for later use
		userDetails.Email = email;
		
		// Start the loading spinner
		startSpinner(START);
		
		// The actual server call
		Server.callWithPOST("/v1/licenses/activate", data, function(status, response)
		{
			// A valid response from the server will return status 200
			
			// ...so if the status is not 200, or the status IS 200 but for whatever reason, the license was not activated
			if(status != 200 || response.activated == false)
			{
				// We'll handle displaying the error to the user
				errorCodeHandling(status, response.error);
				Console.print(status);
			}	
			
			// If the status is 200 and the license has been activated...
			else
			{
				// We'll store the license
				userDetails.License = response.license_key.key;
				// ... and we'll now check that the email the user entered matches the email associated with the license
				checkUserDetails(response);
			}
		});
	}
	
	inline function errorCodeHandling(status, errorDetails)
	{
		lblNotice.showControl(true);
		
		// Handles the errors returned by Lemon Squeezy
		if(status >= 400 && status <= 499)
		{
			// >>> We could get rid of this if statement but without it, if the license entered by the user is not valid,
			// the response looks not very user-friendly.
			// That's your call :)
			if(errorDetails == "license_key not found.")
				lblNotice.set("text", NOTICE.LICENSE_NOT_FOUND);
			else
				lblNotice.set("text", errorDetails);
		}
		
		// Handles other error types
		else if(status >= 500 && status <= 599)
			lblNotice.set("text", NOTICE.SERVER_ERROR);
		
		else if(status == 0)
			lblNotice.set("text", NOTICE.NO_CONNEXION);
		
		// Stop and hide the spinner
		startSpinner(STOP);
	}
	
	inline function checkUserDetails(serverResponse)
	{
		// if the email input matches the email associated with the license key, we can proceed with storing the activation details and tell the user the activation is successful
		if(serverResponse.license_key.status == "active" && serverResponse.meta.customer_email == userDetails.Email)
		{
			// Set the plugin status to 'Activated', and store the instanceID and the username
			userDetails.Activated = true;
			userDetails.InstanceID = serverResponse.instance.id;
			userDetails.Username = serverResponse.meta.customer_name;
			
			// Initialize the registration info file text, and encrypt all the user's details with the variable object we filled along the way
			register.writeString(".");
			register.writeEncryptedObject(userDetails, encryptionKey);
			
			// Stop the spinner
			startSpinner(STOP);
			
			// Show the popup panel to confirm the plugin's successful activation
			pnlActivationSuccess.showControl(true);
			
			//fetchAndDisplayUserDetails();
		}
		else
		{
			errorCodeHandling(400, NOTICE.EMAIL_MISMATCH);
		}
	}
	
	inline function valideActivatedLicense(storedData)
	{
		// Properly format the data we'll send to the API endpoint
		local param = 
		{
			license_key: storedData.License,
			instance_id: storedData.InstanceID
		};
		
		// Reach the API endpoint. If there's an error regarding the license, it will display the error
		// If there's no error returned by the server, or if for some reason the server (or the connexion) is unavailable, we'll do nothing
		// THIS ONE WOULD REQUIRE SOME TESTING <<<<
		Server.callWithPOST("/v1/licenses/validate", param, function(status, response)
		{
			// Using 'var' because you 'Can't reference local variables in nested function body'
			// We'll use this to compare the stored email to the email associated with the license
			var decryptedData = register.loadEncryptedObject(encryptionKey);
			var userEmail = decryptedData.Email;

			if((status >= 400 && status <= 499) || response.error != null || response.license_key.status != "active" || response.meta.customer_email != userEmail)
			{
				errorCodeHandling(status, response.error);
				setValidLicense(false);
			}	
		});
	}
	
	// This function will be executed every time the user load the plugin : it checks locally if the plugins has been marked as 'activated' and that the userData file contains a license
	// If the plugin has not been activated, it will display the activation panel
	inline function checkOnLoad()
	{
		local decryptedData = register.loadEncryptedObject(encryptionKey);
		
		if(decryptedData.Activated)
		{
		    setValidLicense(true);
			
			// You can use this function to check that the license is active on your side and owned by the user, by reaching the API endpoint, each thime the plugin is launched
			// Uncomment this line to enable the online validation process
		    valideActivatedLicense(decryptedData);
		    
		    // Uncomment this line to fetch user details on launch
		    //fetchAndDisplayUserDetails();
		}
		else
		{
			setValidLicense(false);
		}
	}
	
	// This function allows running the plugin
	inline function setValidLicense(isValid)
	{
	    // Assuming you're using a MIDI Muter...
	    MidiMuter1.setAttribute(0, 1 - isValid);
	    pnlActivationWrapper.showControl( 1 - isValid);
	}
	
	// This is the function triggered by the "OK" button that appears if the activation is successful
	inline function onbtnCloseAfterActivationControl(component, value)
	{
		if(!value)
			return;
		
		pnlActivationWrapper.showControl(false);
		pnlActivationSuccess.showControl(false);
	};
	btnCloseAfterActivation.setControlCallback(onbtnCloseAfterActivationControl);
	
	// You can use a function like this if you want to display the user license details on the interface:
	/*
	inline function fetchAndDisplayUserDetails();
	{
		local decryptedData = register.loadEncryptedObject(encryptionKey);
		
		lblDisplayName.set("text", "Registered to: " + decryptedData.userDetails.Username);
		lblDisplayLicense.set("text", "License Number: " + decryptedData.userDetails.License);
	}
	*/
}

//Activation.checkOnLoad();

namespace ActivationDesign
{
	//PANEL WAIT PAINT ROUTINE & TIMER
	const PaintSpinner = function(g)
	{   
	    var a = this.getLocalBounds(0);
	    var w = a[2];
	    var h = a[3];
	    
	    for (i = 0; i < 10; i++)
	    {
	        this.getValue() == i ? g.setColour(this.get("itemColour")) : g.setColour(this.get("bgColour"));
	        
	        var x = w/2;
	        var y1 = a[1];
	        var y2 = h/5;        
	        
	        g.drawLine(x, x, y1, y2, 4);
	        g.rotate(Math.toRadians(360 / 10), [w/2, h/2]);
	    }    
	};
	Activation.pnlWait.setPaintRoutine(PaintSpinner);
	
	const TimeSpinner = function()
	{
	    var v = (this.getValue() + 1) % 10;
	    this.setValue(v);
	    this.repaint();
	};
	Activation.pnlWait.setTimerCallback(TimeSpinner);
}