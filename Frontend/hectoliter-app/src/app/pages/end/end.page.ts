import { Component, OnInit } from '@angular/core';
import { Router, ActivatedRoute } from '@angular/router';
import { timestamp } from 'rxjs';
import { BackendService } from 'src/app/backend.service';

@Component({
  selector: 'app-end',
  templateUrl: './end.page.html',
  styleUrls: ['./end.page.scss'],
})
export class EndPage implements OnInit {

  onPage: boolean = false;

  process: any;

  errorList: any = [];
  processData = {
    status: 0,
    timestamp: "Loading...",
    user: "Loading...",
    weight: "Loading...",
    hectoliter: "Loading...",
    errors: this.errorList
  }
  processSub: any;

  constructor(private router: Router, private backend: BackendService, private route: ActivatedRoute) {
  }

  ionViewDidEnter(){
    this.onPage = true;
    if(!this.backend.checkAuth()){
      return;
    }

    this.processData = {
      status: 0,
      timestamp: "Loading...",
      user: "Loading...",
      weight: "Loading...",
      hectoliter: "Loading...",
      errors: []
    }

    this.onOpenPage();
  }

  ionViewWillLeave(){
    this.onPage = false;
    this.processData = {
      status: 0,
      timestamp: "Loading...",
      user: "Loading...",
      weight: "Loading...",
      hectoliter: "Loading...",
      errors: []
    }
  }

  ngOnInit() {
    this.processSub = this.route.params.subscribe((params: any) =>{
        this.process = params['process'];
      }
    );
  }

  ngOnDestroy(){
    this.processSub.unsubscribe();
  }

  GoToMain() {
    this.router.navigate(['/main']);
  }

  GoToHistory() {
    this.router.navigate(['/history']);
  }

  async onOpenPage(){
    try{
      if(!this.process){
        throw "An error ocurred while processing your request";
      }

      let _process: any = await this.backend.doGet("/processes/"+this.process.toString());
      if(!_process || !('status' in _process) || !(_process.status == 1 || _process.status == 2) || !('timestamp' in _process) || !('user' in _process) || !('hectoliter' in _process) || !('weight' in _process)){
        throw "Invalid response from server. Please contact suport"
      }

      if(_process.status == 2 && (!('error' in _process) || !Array.isArray(_process.error))){
        throw "Invalid response from server. Please contact suport";
      }

      let _errors: any = [];
      if(_process.status == 2){
        _process.error.forEach((el: any) =>{
          _errors.push(el.description??el);
        });
      }

      this.processData = {
        status: _process.status,
        user: _process.user,
        weight: _process.weight.toString() + 'g',
        hectoliter: _process.hectoliter.toString() + 'hL',
        timestamp: new Date(_process.timestamp).toLocaleString(),
        errors: _errors
      }
    }
    catch(err: any){
      this.backend.presentToast(err.error??err);
    }
  }

}
