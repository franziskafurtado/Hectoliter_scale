import { Component, OnInit } from '@angular/core';
import { Router } from '@angular/router';
import { BackendService } from 'src/app/backend.service';

@Component({
  selector: 'app-login',
  templateUrl: './login.page.html',
  styleUrls: ['./login.page.scss'],
})
export class LoginPage implements OnInit {

  user: any;
  pass: any;
  loading: boolean = false;

  constructor(private router: Router, private backend: BackendService) { }

  ngOnInit() {
  }

  async doLogin(){
    try{
      if(!this.user || !this.pass){
        this.backend.presentToast("Username and password must be provided");
        return;
      }

      this.loading = true;

      await this.backend.login(this.user,this.pass);
    }
    catch(err: any){
      this.backend.presentToast(err.error??err);
    }

    this.loading = false;
  }

}
