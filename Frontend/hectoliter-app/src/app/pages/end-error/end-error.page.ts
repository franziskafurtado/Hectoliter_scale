import { Component, OnInit } from '@angular/core';
import { Router } from '@angular/router';

@Component({
  selector: 'app-end-error',
  templateUrl: './end-error.page.html',
  styleUrls: ['./end-error.page.scss'],
})
export class EndErrorPage implements OnInit {

  constructor(private router: Router) { }

  ngOnInit() {
  }

  GoToMain() {
    this.router.navigate(['/main']);
  }

  GoToHistory() {
    this.router.navigate(['/history']);
  }

}
