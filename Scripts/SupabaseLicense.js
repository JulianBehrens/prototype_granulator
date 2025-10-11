namespace SupabaseLicense {
		// Set the base URL we'll use later on when we'll send the activation data
		Server.setBaseURL("https://cjhktgoayqhtpxnegqgi.supabase.co");
		Server.setEnforceTrailingSlash(false);
		
		const API_KEY = "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJzdXBhYmFzZSIsInJlZiI6ImNqaGt0Z29heXFodHB4bmVncWdpIiwicm9sZSI6ImFub24iLCJpYXQiOjE3Mzk0MzMyMzQsImV4cCI6MjA1NTAwOTIzNH0.44MsQlm1SAMWKcTWL6O9Bc_FLmcogw2sUdQTQlZwSfc";
		const PRODUCT_ID = '6010e8e9-5ba9-4f6c-8392-6bcc4a5530ce';	
		
		const Headers = [
		        "Content_Type: application/json",
		        "apikey: " + API_KEY,
		        "Authorization: Bearer " + API_KEY,
		    ];
		var singleHeader = '';
        for (header in Headers) { singleHeader += header + '\n'; }
        Server.setHttpHeader(singleHeader);
		
		
		// anonymous key
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
		
		// ASsuming you're using a MIDI muter
		const MidiMuter1 = Synth.getMidiProcessor("MidiMuter1");
		
		// Declare an array of all possible error messages
		const NOTICE = 
		{
			INVALID_EMAIL : "Invalid Email.",
			INVALID_LICENSE : "Invalid License.",
			LICENSE_NOT_FOUND : "License Key not found.",
			SERVER_ERROR : "Server Error. Please try again or contact support.",
			NO_CONNEXION : "Connection Error. Please check your network settings.",
			EMAIL_MISMATCH : "Wrong Email or License Key",
		}
		
	// ------------------- GUI SETUP ------------------------------------------------------------------------------
	
	// Reference UI elements created manually in HISE interface designer
	const pnlActivationWrapper = Content.getComponent("pnlActivationWrapper");
	const btnAuthorize = Content.getComponent("btnAuthorize");
	const lblInputEmail = Content.getComponent("lblInputEmail");
	const lblInputLicense = Content.getComponent("lblInputLicense");
	const lblNotice = Content.getComponent("lblNotice");
	const pnlWait = Content.getComponent("pnlWait");
	
	// Set default text for input fields
	lblInputEmail.set("text", "e-mail address");
	lblInputLicense.set("text", "license key");
	
	// Constants for spinner
	const START = true;
	const STOP = false;
		
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
			
			//if(license.length != LICENSE_LENGTH || license == "Enter your license..." || license == "")
			//{
			//	lblNotice.showControl(true);
			//	lblNotice.set("text", NOTICE.INVALID_LICENSE);
			//	return false;
			//}
			
			return true;
		}
		// ----------------------------------------------------------------------------------------------------------------------
		
		inline function SubmitLicense(email, license)
		{
			// Get the user inputs and format the into a JSON object
			local data = 
			{
				p_license_key : license,
				p_machine_id: FileSystem.getSystemId(),
				p_product_id: PRODUCT_ID,
				p_email: email
			};
			
			// Store the email the user has entered for later use
			userDetails.Email = email;
			
			// Start the loading spinner
			//startSpinner(START);
			// The actual server call
			Server.callWithPOST("/rest/v1/rpc/activate_license", data, function(status, response)
			{
				// A valid response from the server will return status 200
				Console.print('activate license');
				Console.print(trace(response)); 
				Console.print(status);
				// ...so if the status is not 200, or the status IS 200 but for whatever reason, the license was not activated
				if(response.status_code != 200 || response.success == false)
				{
					// We'll handle displaying the error to the user
					errorCodeHandling(response.status_code, response.message);
				}	
				
				// If the status is 200 and the license has been activated...
				else
				{
					// We'll store the license
					userDetails.License = response.license_id;
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
			if(serverResponse.success) {
			
				Console.print('can activate the plugin');
				// Set the plugin status to 'Activated', and store the instanceID and the username
				userDetails.Activated = true;
				userDetails.activation_time = serverResponse.activation_time;
				
				// Initialize the registration info file text, and encrypt all the user's details with the variable object we filled along the way
				register.writeString(".");
				register.writeEncryptedObject(userDetails, encryptionKey);
				
				// Stop the spinner
				startSpinner(STOP);
				
				// Hide the activation panel after successful activation
				pnlActivationWrapper.showControl(false);
			}
			else {
				errorCodeHandling(400, NOTICE.EMAIL_MISMATCH);
			}
		}
		
		inline function valideActivatedLicense(storedData)
		{
			// Properly format the data we'll send to the API endpoint
			local param = 
			{
				p_license_key: storedData.License,
				p_machine_id: FileSystem.getSystemId()
			};
			
			// Reach the API endpoint. If there's an error regarding the license, it will display the error
			// If there's no error returned by the server, or if for some reason the server (or the connexion) is unavailable, we'll do nothing
			// THIS ONE WOULD REQUIRE SOME TESTING <<<<
			Server.callWithPOST("/rest/v1/rpc/validate_license", param, function(status, response)
			{
				// Using 'var' because you 'Can't reference local variables in nested function body'
				// We'll use this to compare the stored email to the email associated with the license
				//var decryptedData = register.loadEncryptedObject(encryptionKey);
				Console.print('response' + response);
				if (response === 'true') {
					setValidLicense(true);
				} else {
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
				// You can use this function to check that the license is active on your side and owned by the user, by reaching the API endpoint, each thime the plugin is launched
				// Uncomment this line to enable the online validation process
			   	valideActivatedLicense(decryptedData);
			    // simple activation, when there is a license and its active its true.
			    setValidLicense(true);
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
		
		// Success popup removed - no longer needed
		
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
	
	SupabaseLicense.checkOnLoad();
	
}

