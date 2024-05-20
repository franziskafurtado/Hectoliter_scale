import { Component, OnInit } from '@angular/core';
import { Router } from '@angular/router';
import { ToastController } from '@ionic/angular';

@Component({
  selector: 'app-wait',
  templateUrl: './wait.page.html',
  styleUrls: ['./wait.page.scss'],
})
export class WaitPage implements OnInit {

  constructor(private router: Router, private toastController: ToastController) {
    setTimeout(() => {
      this.router.navigate(['/end']); // Mudar para a página desejada após o timeout
    }, 6000);
  }
  async presentToast(position: 'top') {
    const toast = await this.toastController.create({
      message: 'Erro vai aqui',
      duration: 1500,
      position: position,
    });

    await toast.present();
  }

  ngOnInit() {

  }

}