import { Component, OnInit } from '@angular/core';
import { Router } from '@angular/router';

@Component({
  selector: 'app-main',
  templateUrl: './main.page.html',
  styleUrls: ['./main.page.scss'],
})
export class MainPage implements OnInit {

  constructor(private router: Router) { }

  ngOnInit() {
  }
  GoToWait() {
    this.router.navigate(['/wait']);
  }
  GoToHistory() {
    this.router.navigate(['/history']);
  }

}
