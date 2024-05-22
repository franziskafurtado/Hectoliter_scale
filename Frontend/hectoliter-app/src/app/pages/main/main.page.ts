import { Component, OnInit } from '@angular/core';
import { Router } from '@angular/router';
import { BackendService } from 'src/app/backend.service';

@Component({
  selector: 'app-main',
  templateUrl: './main.page.html',
  styleUrls: ['./main.page.scss'],
})
export class MainPage implements OnInit {

  onPage: boolean = false;
  machine: any = null;
  machineAvailable: boolean = false;
  started: boolean = false;

  constructor(private router: Router, private backend: BackendService) { }

  ngOnInit() {
  }

  ionViewDidEnter(){
    this.onPage = true;
    if(!this.backend.checkAuth()){
      return;
    }

    this.onOpenPage();
  }

  ionViewWillLeave(){
    this.onPage = false;
  }

  StartProcess() {
    if(!this.machine){
      this.backend.presentToast("An error ocurred while processing your request");
      return;
    }
    this.router.navigate(['/wait', {machine: this.machine}]);
  }
  GoToHistory() {
    this.router.navigate(['/history']);
  }

  async onOpenPage(){
    try{
      let _machines: any = await this.backend.doGet("/machines");

      if(!_machines || !Array.isArray(_machines) || _machines.length == 0 || !_machines[0].id){
        throw "Invalid response from server. Please contact suport"
      }

      this.machine = _machines[0].id;

      while(this.onPage){
        try{
          let _machine: any = await this.backend.doGet("/machines/" + this.machine.toString());
          if(!_machine || !_machine.lastUpdate || !('status' in _machine)){
            throw "Invalid response from server. Please contact suport"
          }

          this.started = true;

          if(_machine.status == 1 || _machine.status == 2 || (new Date().getTime() - new Date(_machine.lastUpdate).getTime()) > 600000){
            this.machineAvailable = false;
          }
          else{
            this.machineAvailable = true;
          }

          await this.sleep(5000);
        }
        catch(err: any){
          this.backend.presentToast(err.error??err);
          await this.sleep(5000);
        }
      }
    }
    catch(err: any){
      this.backend.presentToast(err.error??err);
      return;
    }
  }

  sleep(ms: any){
    return new Promise(resolve => {
      setTimeout(resolve, ms);
    })
  }

}
