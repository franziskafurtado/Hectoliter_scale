import { Component, OnInit } from '@angular/core';
import { Router, ActivatedRoute } from '@angular/router';
import { ToastController } from '@ionic/angular';
import { BackendService } from 'src/app/backend.service';

@Component({
  selector: 'app-wait',
  templateUrl: './wait.page.html',
  styleUrls: ['./wait.page.scss'],
})
export class WaitPage implements OnInit {

  onPage: boolean = false;
  machineSub: any;
  machine: any;
  step: any;
  processing: boolean = false;
  process: any;

  constructor(private router: Router, private toastController: ToastController, private backend: BackendService, private route: ActivatedRoute) {
    /* setTimeout(() => {
      this.router.navigate(['/end']); // Mudar para a página desejada após o timeout
    }, 6000); */
  }

  ionViewDidEnter(){
    this.onPage = true;
    if(!this.backend.checkAuth()){
      return;
    }
    this.step = 0;
  }

  ionViewWillLeave(){
    this.onPage = false;
  }

  ngOnInit() {
    this.machineSub = this.route.params.subscribe((params: any) =>{
        this.machine = params['machine'];
      }
    );
  }

  ngOnDestroy(){
    this.machineSub.unsubscribe();
  }

  goToMain(){
    this.router.navigate(['/main']);
  }

  async onStartProcess(){
    this.step = 1;
    try{
      if(!this.machine){
        throw "An error ocurred while processing your request";
      }

      let _start: any = await this.backend.doPost('/machines/' + this.machine.toString() + '/start', {});

      if(!_start || !_start.process){
        throw "Invalid response from server. Please contact suport"
      }

      this.step = 2;
      this.processing = true;
      this.process = _start.process;

      const _startTime = new Date();

      let _countErr = 0;
      let _statusChanged = false;

      let _toast: any = null;

      while(this.onPage && this.processing){
        try{
          let _machine: any = await this.backend.doGet("/machines/" + this.machine.toString());
          if(!_machine || !_machine.lastUpdate || !('status' in _machine)){
            if(_countErr >= 60){
              throw {
                error: "Too many server errors! Exiting",
                exit: true
              }
            }
            throw "Invalid response from server. Please contact suport";
          }

          if(_machine.status != 0){
            _statusChanged = true;
          }

          if(!_statusChanged && (new Date().getTime() - _startTime.getTime() > 60000)){
            throw {
              error: "Machine timeout. Exiting",
              exit: true
            }
          }

          if(_toast){
            _toast.dismiss();
            _toast = null;
          }

          if(_machine.status == 2){
            if(!_machine.errors || !Array.isArray(_machine.errors)){
              if(_countErr >= 60){
                throw {
                  error: "Too many server errors! Exiting",
                  exit: true
                }
              }
              throw "Invalid response from server. Please contact suport";
            }

            let _message = "";
            if(_machine.errors.length == 0){
              _message = "An unknown error has ocuured";
            }
            else{
              _machine.errors.forEach((element: any) => {
                _message = _message + element.description + "; ";
              });
            }

            _toast = await this.toastController.create({
              message: _message,
              position: 'top',
            });

            _toast.present();
          }

          let _process: any = await this.backend.doGet("/processes/"+this.process.toString());
          if(!_process || !('status' in _process)){
            if(_countErr >= 60){
              throw {
                error: "Too many server errors! Exiting",
                exit: true
              }
            }
            throw "Invalid response from server. Please contact suport"
          }

          if(_process.status != 0){
            this.processing = false;
            this.router.navigate(['/end', {process: this.process}]);
          }

          await this.sleep(1000);
        }
        catch(err: any){
          if(err.exit){
            throw err;
          }
          this.backend.presentToast(err.error??err);
          _countErr++;
          await this.sleep(1000);
        }
      }
    }
    catch(err: any){
      this.backend.presentToast(err.error??err);
      this.router.navigate(['/main']);
    }
  }

  sleep(ms: any){
    return new Promise(resolve => {
      setTimeout(resolve, ms);
    })
  }

}