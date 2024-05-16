<h2>Software Documentation</h2>

<h2>Functional software requirements</h2>
<b>FR21.</b> The app must authenticate the user before letting them access the machine's information and function. <br> 
<b>FR22.</b> The app should display a wait screen while the process is carried out. <br> 
<b>FR23.</b> The app should display, at the end of the process, all information about the measurement. <br> 
<b>FR24.</b> The app must have a button to request the start of a process. <br> 
<b>FR25.</b> The app must have a history screen, that shows all previous measurement results. <br> 
<b>FR26.</b> The app must allow the user to filter the history screen data by dates and display a list of measurements within that period. <br> 
<b>FR27</b> The app’s start button must only work if the machine is available (has returned the measurement information from the previous request). <br> 

<h2>Non-functional software requirements</h2>
<b>NFR13.</b>The app will be developed in Ionic framework.<br> 
<b>NFR14.</b>The app will have an initial screen after logging in, to choose between the options of looking at the history and starting a new measurement process.<br> 
<b>NFR15.</b>The database must be a SQL one.<br> 
<b>NFR16.</b>The database must be accessible for the app and the machine via REST API.<br> 
<b>NFR17.</b>The REST API must be developed in the Python language.<br> 
<b>NFR18.</b>The database and API must be hosted in the cloud.<br> 

<h2>UML diagrams</h2>

<h2><b>App</b></h2>

<b>Use Case</b>

<div align="center">
    <img  width="800" src="use-case.jpeg"/>
</div>
<br>
<b>Activities</b>

<div align="center">
    <img  width="800" src="Atividade.jpeg"/>
</div>
<br>

<b>Sequence Diagram - Main</b>

<div align="center">
    <img  width="800" src="sequence_diagram_main.jpeg"/>
</div>

<br> 

<b>Sequence Diagram - History</b>

<div align="center">
    <img  width="800" src="sequence_diagram_history.jpeg"/>
</div>

<br> 
<b>App Screens</b>
<div align="center">

    

https://github.com/franziskafurtado/Hectoliter_scale/assets/23341934/ccec0c3b-bf57-425f-8323-3eff5aa60e68


    
</div>
<h3>Back</h3>

<b>Deploy</b>

<div align="center">
    <img  width="800" src="deploy.jpg"/>
</div>
<br>

<b>Relational</b>
<div align="center">
    <img  width="800" src="relacional.jpg"/>
</div>
<br>

<b>Seq Machine</b>
<div align="center">
    <img  width="800" src="seqmachine.jpg"/>
</div>


<br>

<h3>Firmware</h3>

<b>Use Case</b>
<br>
![UseCaseDiagram_FirmwareView](https://github.com/franziskafurtado/Hectoliter_scale/assets/47851647/0f07d758-3e74-47b4-a4dd-6b814fd3c7d5)

<br>

<b>Full Firmware State Machine Chart</b>
<br>
![FirmwareLogic_GeneralIdea](https://github.com/franziskafurtado/Hectoliter_scale/assets/47851647/18858bcd-5b43-4fd9-b48c-5b29b5475449)

<br>

<b>Collecting Phase State Flowchart Execution</b>
<br>
![FirmwareLogic_CollectingPhase](https://github.com/franziskafurtado/Hectoliter_scale/assets/47851647/34785664-571e-4ccd-855c-4cc4352da1fc)

<br>

<b>Measuring Phase State Flowchart Execution</b>
<br>
![FirmwareLogic_MeasuringPhase](https://github.com/franziskafurtado/Hectoliter_scale/assets/47851647/a5755371-43c4-4b51-8ff7-1f527b0c15d1)

<br>

<b>Returning Phase State Flowchart Execution</b>
<br>
![FirmwareLogic_ReturningPhase](https://github.com/franziskafurtado/Hectoliter_scale/assets/47851647/4e33c36a-4d33-4796-a1cf-1c40997d7736)

<br>


<br>


<h3>App Error Codes</h3><br> 

<b>10.</b> Servo Motor 1 is stuck<br> 
<b>20.</b> Servo Motor 2 is stuck<br> 
<b>30.</b> Stepper motor is stuck<br> 
<b>40.</b> Return container has been removed<br> 
<b>50.</b> The sample is not enough<br>
<b>60.</b> Collecting container was opened<br>  
<b>70.</b> Connection error<br> 
<b>80.</b> Time out<br> 







