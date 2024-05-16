import { Component, OnInit } from '@angular/core';
import { Router } from '@angular/router';

@Component({
  selector: 'app-wait',
  templateUrl: './wait.page.html',
  styleUrls: ['./wait.page.scss'],
})
export class WaitPage implements OnInit {

  constructor(private router: Router) {
    setTimeout(() => {
      this.router.navigate(['/end']); // Mudar para a página desejada após o timeout
    }, 6000);
  }

  ngOnInit() {
  }

}
