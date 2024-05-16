import { ComponentFixture, TestBed } from '@angular/core/testing';
import { EndErrorPage } from './end-error.page';

describe('EndErrorPage', () => {
  let component: EndErrorPage;
  let fixture: ComponentFixture<EndErrorPage>;

  beforeEach(() => {
    fixture = TestBed.createComponent(EndErrorPage);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
