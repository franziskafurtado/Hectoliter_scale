import { Component, OnInit } from '@angular/core';
import { Router } from '@angular/router';
import { BackendService } from 'src/app/backend.service';

@Component({
  selector: 'app-end-error',
  templateUrl: './end-error.page.html',
  styleUrls: ['./end-error.page.scss'],
})
export class EndErrorPage implements OnInit {

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

  GoToHistory() {
    this.router.navigate(['/history']);
  }

}
