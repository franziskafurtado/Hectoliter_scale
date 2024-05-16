import { Component, OnInit } from '@angular/core';
import { Router } from '@angular/router';

@Component({
  selector: 'app-end',
  templateUrl: './end.page.html',
  styleUrls: ['./end.page.scss'],
})
export class EndPage implements OnInit {

  constructor(private router: Router) {
    setTimeout(() => {
      this.router.navigate(['/end-error']); // Mudar para a página desejada após o timeout
    }, 6000);
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
