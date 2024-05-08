Software design with UML diagrams (use-case, sequence, activity, classes, deployment, entity-relationship), for firmware, frontend and backend, and mobile app.

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

<b>App</b>

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
<b>Screen Sequence</b>

<div align="center">
    <img  width="800" src="sequenciadetelas.jpeg"/>
</div>

<br> 

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


