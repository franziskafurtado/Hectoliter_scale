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

  constructor(private router: Router, private backend: BackendService) { }

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

  GoToEnd() {
    this.router.navigate(['/end', {process: 5}]);
  }
}
