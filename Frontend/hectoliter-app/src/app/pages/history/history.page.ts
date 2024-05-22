import { Component, OnInit } from '@angular/core';
import { Router } from '@angular/router';
import { BackendService } from 'src/app/backend.service';

@Component({
  selector: 'app-history',
  templateUrl: './history.page.html',
  styleUrls: ['./history.page.scss'],
})
export class HistoryPage implements OnInit {

  onPage: boolean = false;

  historyItens: any[] = [];
  dataIni: any;
  dataFim: any;
  loading: boolean = false;

  constructor(private router: Router, private backend: BackendService) { }

  ionViewWillEnter(){
    this.dataIni = new Date().toISOString();
    this.dataFim = new Date().toISOString();
    this.historyItens = [];
  }

  ionViewDidEnter(){
    this.onPage = true;
    if(!this.backend.checkAuth()){
      return;
    }
  }

  ionViewWillLeave(){
    this.onPage = false;
  }

  ngOnInit() {
  }

  GoToMain() {
    this.router.navigate(['/main']);
  }

  GoToProcess(id: any) {
    this.router.navigate(['/end', {process: id}]);
  }

  async onSearch(){
    let _dataIni = this.formatData(this.dataIni);
    let _dataFim = this.formatData(this.dataFim);

    let _url = '/processes';

    if(_dataIni && !_dataFim){
      _url = _url + '?start=' + _dataIni;
    }
    else if(!_dataIni && _dataFim){
      _url = _url + '?end=' + _dataFim;
    }
    else if(_dataIni && _dataFim){
      _url = _url + '?start=' + _dataIni + '&end=' + _dataFim;
    }

    try{
      this.loading = true;
      let _processes = await this.backend.doGet(_url);
      if(!_processes || !Array.isArray(_processes)){
        throw "Invalid response from server. Please contact suport"
      }

      this.historyItens = _processes.map((element: any) => {
        return {
          id: element.id,
          status: element.status,
          timestamp: new Date(element.timestamp).toLocaleString()
        }
      });

    }
    catch(err: any){
      this.backend.presentToast(err.error??err);
    }

    this.loading = false;
  }

  formatData(iso: any){
    let _data: any = iso ? new Date(iso) : null;

    if(_data){
      let _year: any = _data.getFullYear().toString();
      let _month: any = _data.getMonth() + 1;
      let _day: any = _data.getDate();

      if(_month < 10){
        _month = '0' + _month.toString();
      }
      else{
        _month = _month.toString();
      }

      if(_day < 10){
        _day = '0' + _day.toString();
      }
      else{
        _day = _day.toString();
      }

      _data = _year + '-' + _month + '-' + _day;
    }

    return _data;
  }
}
