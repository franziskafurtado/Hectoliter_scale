import { Injectable } from '@angular/core';
import { HttpClient, HttpHeaders } from '@angular/common/http';
import { Router } from '@angular/router';
import { ToastController } from '@ionic/angular';

@Injectable({
  providedIn: 'root'
})
export class BackendService {


  baseUrl: string = "https://joaopedrogalera.pythonanywhere.com";
  token: string = "";

  constructor(private httpClient: HttpClient, private router: Router, private toastController: ToastController) { }

  async login(user: any, pass: any){
    let _body = new URLSearchParams();
    _body.set('username', user);
    _body.set('password', pass);

    let _options = {
      headers: new HttpHeaders().set('Content-Type', 'application/x-www-form-urlencoded')
    };

    try{
      const _token: any = await this.doLogin(_options, _body.toString());

      if(!_token || !_token.token){
        throw {error: "invalid response from server"}
      }

      this.token = _token.token;
      this.router.navigate(['/main']);
    }
    catch(err: any){
      this.presentToast(err.error??err);
    }

  }

  doLogin(options: any, body: any){
    return new Promise((resolve, reject) => {
      this.httpClient.post(this.baseUrl + "/login", body, options).subscribe({
      next: (response: any) => {
        resolve(response);
      },
      error: (error) => {
        reject(error.error);
      }});
    });
  }

  async doPost(url: any, body: any){
    if(!this.token){
      this.router.navigate(['/login']);
      throw "User not authenticated"
    }

    let _options = {
      headers: new HttpHeaders().set('Token', this.token)
    };

    try{
      return await new Promise((resolve, reject) => {
        this.httpClient.post(this.baseUrl + url, body, _options).subscribe({
        next: (response: any) => {
          resolve(response);
        },
        error: (error) => {
          reject(error);
        }});
      });
    }
    catch(err: any){
      if(err.status && err.status == 401){
        this.router.navigate(['/login']);
      }
      throw err.error??err;
    }
  }

  async doGet(url: any){
    if(!this.token){
      this.router.navigate(['/login']);
      throw "User not authenticated"
    }

    let _options = {
      headers: new HttpHeaders().set('Token', this.token)
    };

    try{
      return await new Promise((resolve, reject) => {
        this.httpClient.get(this.baseUrl + url, _options).subscribe({
        next: (response: any) => {
          resolve(response);
        },
        error: (error) => {
          reject(error);
        }});
      });
    }
    catch(err: any){
      if(err.status && err.status == 401){
        this.router.navigate(['/login']);
      }
      throw err.error??err;
    }
  }

  async presentToast(message: any) {
    const toast = await this.toastController.create({
      message: message,
      duration: 2500,
      position: 'top',
    });

    toast.present();
  }

  checkAuth(){
    if(!this.token){
      this.presentToast("User not authenticated");
      this.router.navigate(['/login']);
      return false;
    }

    return true;
  }
}
