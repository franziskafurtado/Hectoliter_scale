import { Component, OnInit } from '@angular/core';
import { Router } from '@angular/router';

@Component({
  selector: 'app-history',
  templateUrl: './history.page.html',
  styleUrls: ['./history.page.scss'],
})
export class HistoryPage implements OnInit {

  constructor(private router: Router) { }

  ngOnInit() {
  }

  GoToMain() {
    this.router.navigate(['/main']);
  }

  GoToEnd() {
    this.router.navigate(['/end']);
  }
}
